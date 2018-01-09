#include <event2/event.h>

#include "common/header.h"
#include "common/util.h"
#include "common/log.h"

void supportedMethod();
event_config* getEventConfig();
void timeout(event_base*);

int main() {
    supportedMethod();

    event_config* ev_cfg = getEventConfig();
    event_base* ev_base = event_base_new_with_config(ev_cfg);
    LOG("new event_base: %s", event_base_get_method(ev_base));
    event_config_free(ev_cfg);

    timeout(ev_base);

    event_base_dispatch(ev_base);
    event_base_free(ev_base);
    return 0;
}

void supportedMethod() {
    const char** methods = event_get_supported_methods();
    printf("supported methods:");
    for (int i = 0; methods[i] != NULL; ++i) {
        printf(" %s", methods[i]);
    }
    printf("\n");
}

event_config* getEventConfig() {
    event_config* cfg = event_config_new();
    // if (cfg == NULL)

    // 设置选择event_method时, 应该具有的特征
    if (event_config_require_features(cfg,
            // event_method_feature::EV_FEATURE_ET |  // 边缘触发
            // event_method_feature::EV_FEATURE_O1 |  // 激活一个事件的时间复杂度是O(1)
            event_method_feature::EV_FEATURE_FDS  // 支持套接字外的fd, poll-method
        ) != 0) {
        LOG("config method features fail");
    }

    // event_base的配置, event_base使用时不加锁
    if (event_config_set_flag(cfg, event_base_config_flag::EVENT_BASE_FLAG_NOLOCK) != 0) {
        LOG("config event_base flag fail");
    }
    return cfg;
}

void timeout(event_base* ev_base) {
    using namespace std::placeholders;

    timeval two_secs = { 2, 0 };
    const timeval* tv_out = event_base_init_common_timeout(ev_base, &two_secs);
    LOG("tv_out: %ld.%ld", tv_out->tv_sec, tv_out->tv_usec);
    memcpy(&two_secs, tv_out, sizeof(timeval));

    static util::time_point gtl_start_time;
    gtl_start_time = util::now();

    // 不能通过cb捕获gtl_start_time, 否则cb不能转换成event_callback_fn
    // lambda with captures相当于一个类的实例, 调用时需要用到this指针, 所以不能转成c-style function pointer
    auto cb = [](evutil_socket_t fd, short revents, void* arg) {
        if (revents & EV_TIMEOUT) {
            auto timept = util::now();
            LOG("timeout: %ld", util::timeDiff(timept, gtl_start_time));
        }
        if (arg != nullptr) {
            event_free((event*)arg);
        }
    };

    event* ev = event_new(ev_base, -1, EV_TIMEOUT, cb, event_self_cbarg());
    event_add(ev, &two_secs);
}