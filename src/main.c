/*
    Authors:
        Pavel Březina <pbrezina@redhat.com>

    Copyright (C) 2016 Red Hat

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _GNU_SOURCE
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <unistd.h>

#define PAM_TEST_SERVICE "pam_test"
#define PAM_TEST_STACK "/etc/pam.d/" PAM_TEST_SERVICE

#define CHECK_PAM_STATUS(what, handle, ret, msgs, i, label) do {            \
    asprintf(&msgs[i++], "%s returned [%d]: %s",                            \
             what, ret, pam_strerror(handle, ret));                         \
    if (ret != PAM_SUCCESS) {                                               \
        goto label;                                                         \
    }                                                                       \
} while (0)

enum pam_test_op {
    PAM_TEST_AUTH,
    PAM_TEST_CHPASS,

    PAM_TEST_INVALID
};

static enum pam_test_op parse_operation(const char *operation)
{
    if (strcmp("auth", operation) == 0) {
        return PAM_TEST_AUTH;
    } else if (strcmp("chpass", operation) == 0) {
        return PAM_TEST_CHPASS;
    }

    return PAM_TEST_INVALID;
}


static void print_pam_stack(const char *stack)
{
    FILE *file;
    int c;

    file = fopen(stack, "r");
    if (file == NULL) {
        return;
    }

    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    putchar('\n');

    fclose(file);

    return;
}

static int test_auth(const char *user)
{
    pam_handle_t *pamh;
    char *msgs[4] = {0};
    int i = 0;
    int ret;

    static struct pam_conv conv = {
        misc_conv,
        NULL
    };

    ret = pam_start(PAM_TEST_SERVICE, user, &conv, &pamh);
    CHECK_PAM_STATUS("pam_start", pamh, ret, msgs, i, done);

	ret = pam_set_item(pamh, PAM_RUSER, strdup(cuserid(NULL)));
	CHECK_PAM_STATUS("pam_set_item:PAM_RUSER", pamh, ret, msgs, i, done);

    ret = pam_authenticate(pamh, 0);
    CHECK_PAM_STATUS("pam_authenticate", pamh, ret, msgs, i, done);

    ret = pam_acct_mgmt(pamh, 0);
    CHECK_PAM_STATUS("pam_acct_mgmt", pamh, ret, msgs, i, done);

    ret = pam_end(pamh, ret);
    CHECK_PAM_STATUS("pam_end", pamh, ret, msgs, i, done);

    ret = PAM_SUCCESS;

done:
    printf("\n\n");

    for (i = 0; i < sizeof(msgs) / sizeof(const char*); i++) {
        if (msgs[i] == NULL) {
            continue;
        }

        puts(msgs[i]);
        free(msgs[i]);
    }

    return ret;
}

static int test_chpass(const char *user)
{
    pam_handle_t *pamh;
    char *msgs[4] = {0};
    int i = 0;
    int ret;

    static struct pam_conv conv = {
        misc_conv,
        NULL
    };

    ret = pam_start(PAM_TEST_SERVICE, user, &conv, &pamh);
    CHECK_PAM_STATUS("pam_start", pamh, ret, msgs, i, done);

    ret = pam_authenticate(pamh, 0);
    CHECK_PAM_STATUS("pam_authenticate", pamh, ret, msgs, i, done);

    ret = pam_acct_mgmt(pamh, 0);
    CHECK_PAM_STATUS("pam_acct_mgmt", pamh, ret, msgs, i, done);

    ret = pam_chauthtok(pamh, 0);
    CHECK_PAM_STATUS("pam_chauthtok", pamh, ret, msgs, i, done);

    ret = pam_end(pamh, ret);
    CHECK_PAM_STATUS("pam_end", pamh, ret, msgs, i, done);

    ret = PAM_SUCCESS;

done:
    printf("\n\n");

    for (i = 0; i < sizeof(msgs) / sizeof(const char*); i++) {
        if (msgs[i] == NULL) {
            continue;
        }

        puts(msgs[i]);
        free(msgs[i]);
    }

    return ret;
}

int main(int argc, const char *argv[])
{
    enum pam_test_op op;
    const char *user;
    int ret;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s [auth|chpass] @username\n", argv[0]);
        return 1;
    }


    user = argv[2];

    if (!getenv("PAM_SKIP_CHECK_CONFIG")) {
      /* PAM Stack for the service must exist. */
      if(access("/etc/pam.d/" PAM_TEST_SERVICE, F_OK) != 0) {
        fprintf(stderr, PAM_TEST_STACK " does not exist!\n");
        return 1;
      }
      print_pam_stack(PAM_TEST_STACK);
    }

    op = parse_operation(argv[1]);
    switch (op) {
    case PAM_TEST_AUTH:
        ret = test_auth(user);
        if (ret == PAM_SUCCESS) {
            printf("\nAuthentication successful.\n");
        } else {
            printf("\nAuthentication failed.\n");
        }
        break;
    case PAM_TEST_CHPASS:
        ret = test_chpass(user);
        if (ret == PAM_SUCCESS) {
            printf("\nPassword changed.\n");
        } else {
            printf("\nPassword change failed.\n");
        }
        break;
    default:
        fprintf(stderr, "Invalid operation!\n");
        return 1;
    }

    return ret;
}
