#include <string.h>
#include <stdlib.h>

#include <mcabber/logprint.h>
#include <mcabber/modules.h>
#include <mcabber/hooks.h>
#include <mcabber/xmpp.h>
#include <mcabber/commands.h>
#include <mcabber/settings.h>

#define UNUSED(x) (void)(x)

static void autojoin_init(void);
static void autojoin_uninit(void);

static unsigned int g_hook;

module_info_t info_autojoin = {
    .branch          = MCABBER_BRANCH,
    .api             = MCABBER_API_VERSION,
    .version         = "1.0",
    .description     = "Provides automatic join to rooms without bookmarks",
    .requires        = NULL,
    .init            = autojoin_init,
    .uninit          = autojoin_uninit,
    .next            = NULL,
};

static guint autojoin_on_connect(const gchar *hook, hk_arg_t *args, gpointer data) {
    UNUSED(hook);
    UNUSED(args);
    UNUSED(data);

    scr_log_print(LPRINT_NORMAL, "autojoin: connected");

    const char *mucs_option = (const char *) settings_get(
        SETTINGS_TYPE_OPTION,
        "autojoin"
    );
    if (!mucs_option) {
        return HOOK_HANDLER_RESULT_ALLOW_MORE_HANDLERS;
    }

    char *mucs = strdup(mucs_option);
    if (!mucs) {
        scr_log_print(LPRINT_NORMAL, "autojoin: out of memory");
        return HOOK_HANDLER_RESULT_ALLOW_MORE_HANDLERS;
    }

    char* muc_name = strtok(mucs, ",");

    char* server_name;

    do {
        char full_muc_name[1024];

        if (strstr(muc_name, "@")) {
            server_name = strstr(muc_name, "@");

            snprintf(
                full_muc_name,
                sizeof(full_muc_name),
                "%s",
                muc_name
            );
        } else {
            snprintf(
                full_muc_name,
                sizeof(full_muc_name),
                "%s%s",
                muc_name, server_name
            );
        }

        scr_log_print(LPRINT_NORMAL,
            "autojoin: joining to %s", full_muc_name
        );

        xmpp_room_join(full_muc_name, default_muc_nickname(muc_name), "");
    } while ((muc_name = strtok(NULL, ",")) != NULL);

    free(mucs);

    return HOOK_HANDLER_RESULT_ALLOW_MORE_HANDLERS;
}

static void autojoin_init(void) {
    scr_log_print(LPRINT_NORMAL, "autojoin: init");

    hk_add_handler(
        autojoin_on_connect,
        HOOK_POST_CONNECT,
        G_PRIORITY_DEFAULT,
        NULL
    );
}

static void autojoin_uninit(void) {
    hk_del_handler(HOOK_POST_CONNECT, g_hook);
}
