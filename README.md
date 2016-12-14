# PAM Test

This application can be used to test a PAM stack for authentication
and password change.

# Example

1. Compile the source.

```bash
$ cd /source/dir
$ make
```

## Using the application

2. Create a PAM stack at `/etc/pam.d/pam_test`
3. Test authentication

```bash
$ ./pam_test auth pam-test
auth        required      pam_env.so
auth        sufficient    pam_unix.so nullok try_first_pass
auth        requisite     pam_succeed_if.so uid >= 1000 quiet_success
auth        required      pam_deny.so

account     required      pam_unix.so
account     sufficient    pam_localuser.so
account     sufficient    pam_succeed_if.so uid < 1000 quiet
account     required      pam_permit.so

password    requisite     pam_pwquality.so try_first_pass local_users_only retry=3 authtok_type=
password    sufficient    pam_unix.so md5 shadow nullok try_first_pass use_authtok
password    required      pam_deny.so

session     optional      pam_keyinit.so revoke
session     required      pam_limits.so
-session     optional      pam_systemd.so
session     [success=1 default=ignore] pam_succeed_if.so service in crond quiet use_uid
session     required      pam_unix.so


Password: 


pam_start returned [0]: Success
pam_authenticate returned [0]: Success
pam_acct_mgmt returned [0]: Success
pam_end returned [0]: Success

Authentication successful.
```

3. Test password change

```bash
$ ./pam_test chpass pam-test
auth        required      pam_env.so
auth        sufficient    pam_unix.so nullok try_first_pass
auth        requisite     pam_succeed_if.so uid >= 1000 quiet_success
auth        required      pam_deny.so

account     required      pam_unix.so
account     sufficient    pam_localuser.so
account     sufficient    pam_succeed_if.so uid < 1000 quiet
account     required      pam_permit.so

password    requisite     pam_pwquality.so try_first_pass local_users_only retry=3 authtok_type=
password    sufficient    pam_unix.so md5 shadow nullok try_first_pass use_authtok
password    required      pam_deny.so

session     optional      pam_keyinit.so revoke
session     required      pam_limits.so
-session     optional      pam_systemd.so
session     [success=1 default=ignore] pam_succeed_if.so service in crond quiet use_uid
session     required      pam_unix.so


Password: 
Changing password for pam-test.
(current) UNIX password: 
New password: 
Retype new password: 


pam_start returned [0]: Success
pam_authenticate returned [0]: Success
pam_acct_mgmt returned [0]: Success
pam_chauthtok returned [0]: Success

Password changed.
```

## Using shell script

```bash
$ . pam_test.sh
$ pam-test-auth --help
Test selected pam stack against authentication.

PAM stack @STACK is copied to /etc/pam.d/pam_test and pam_test 
application is run with @USERNAME. @PASSWORD is automatically 
passed to standard input.

Usage:
pam-test-auth STACK USERNAME PASSWORD

$ pam-test-chpass --help
Test selected pam stack against password change.

PAM stack @STACK is copied to /etc/pam.d/pam_test and pam_test 
application is run with @USERNAME. @PASSWORD is automatically 
passed to standard input.

Usage:
pam-test-chpass STACK USERNAME PASSWORD NEWPASSWORD
```

# PAM Modules

This is a basic description of commonly used PAM modules.

* **pam_env** -- set environment variables
```
auth, session
```
* **pam_unix** -- local authentication from `/etc/passwd`
```
access, auth, password, session
```
* **pam_succeed_if** -- test account characteristics
```
access, auth, password, session
```
* **pam_deny** -- always fail
```
access, auth, password, session
```
* **pam_localuser** -- success if it is a local user from `/etc/passwd`
```
access, auth, password, session
```
* **pam_permit** -- always succeed
```
access, auth, password, session
```
* **pam_keyinit** -- initialize kernel session keyring
```
session
```
* **pam_limits** -- limit resources per setting in `limits.conf`
```
session
```
* **pam_systemd** -- register user session with systemd login manager
```
session
```
* **pam_sss** -- authenticate against SSSD
```
access, auth, password, session
```
* **pam_ldap** -- authenticate against LDAP
```
access, auth, password, session
```
* **pam_krb5** -- authenticate against Kerberos
```
access, auth, password, session
```
* **pam_winbind** -- authenticate against Winbind
```
access, auth, password, session
```
* **pksc11** -- authenticate using a smartcard
```
auth
```
* **pam_fprintd** -- authenticate using a fingerprint
```
auth
```
* **pam_ecryptfs** -- unwrap ecrypt passphrase and mount encrypted directory
```
auth, session
```
* **pam_oddjob_mkhomedir** -- create home directory if it does not exist yet
```
session
```
* **pam_pwquality** -- force password policy
```
password
```
* **pam_faillock** -- lock account if authentication failed too many times
```
auth, account
```