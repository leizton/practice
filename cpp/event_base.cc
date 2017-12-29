#include <event2/event.h>

#include "common/header.h"

void supportedMethod();
event_config* getEventConfig();

int main() {
    supportedMethod();

    event_config* ev_cfg = getEventConfig();
    event_base* ev_base = event_base_new_with_config(ev_cfg);
    printf("new event_base: %s\n", event_base_get_method(ev_base));

    return 0;
}

void supportedMethod() {
    const char** methods = event_get_supported_methods();
    printf("supported methods: ");
    for (int i = 0; methods[i] != NULL; ++i) {
        printf("%s ", methods[i]);
    }
    printf("\n");
}

event_config* getEventConfig() {
    event_config* cfg = event_config_new();
    // if (cfg == NULL)

    int ret = event_config_require_features(cfg,
        //event_method_feature::EV_FEATURE_ET |  // 边缘触发
        //event_method_feature::EV_FEATURE_O1 |  // 激活一个事件的时间复杂度是O(1)
        event_method_feature::EV_FEATURE_FDS  // 支持套接字外的fd
    );

    event_config_set_flag(cfg, event_base_config_flag::EVENT_BASE_FLAG_NOLOCK);  // 不给event_base分配锁
    return cfg;
}