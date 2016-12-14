#!/bin/bash

should-print-help() {
    if [[ $# -eq 0 ||  "$1" == "-h" || "$1" == "--help" ]]
    then
        return 0
    fi
    
    return 1
}

pam-test-auth() {
    if should-print-help $@
    then 
        echo "Test selected pam stack against authentication."
        echo ""
        echo "PAM stack @STACK is copied to /etc/pam.d/pam_test and pam_test "
        echo "application is run with @USERNAME. @PASSWORD is automatically "
        echo "passed to standard input."
        echo ""
        echo "Usage:"
        echo "$0 STACK USERNAME PASSWORD" 
        echo ""
        return 0
    fi

    local STACK=$1
    local USERNAME=$2
    local PASSWORD=$3
 
    sudo cp -f $STACK /etc/pam.d/pam_test \
        && echo "$PASSWORD" | ./pam_test auth $USERNAME
}

pam-test-chpass() {
    if should-print-help $@
    then 
        echo "Test selected pam stack against password change."
        echo ""
        echo "PAM stack @STACK is copied to /etc/pam.d/pam_test and pam_test "
        echo "application is run with @USERNAME. @PASSWORD is automatically "
        echo "passed to standard input."
        echo ""
        echo "Usage:"
        echo "$0 STACK USERNAME PASSWORD NEWPASSWORD" 
        echo ""
        return 0
    fi

    local STACK=$1
    local USERNAME=$2
    local PASSWORD=$3
    local NEWPASSWORD=$4
 
    sudo cp -f $STACK /etc/pam.d/pam_test \
        && echo "$PASSWORD" | ./pam_test auth $USERNAME
}