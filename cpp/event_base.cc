#include "common/header.h"

#include <event2/event.h>

void supportedMethod();
event_config* getEventConfig();

int main() {
    supportedMethod();

    event_config* cfg = getEventConfig();

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
        event_method_feature::EV_FEATURE_ET |  // 边缘触发
        event_method_feature::EV_FEATURE_O1 |  // 时间复杂度是O(1)
        event_method_feature::EV_FEATURE_FDS   // 支持套接字外的fd
    );

    return cfg;
}