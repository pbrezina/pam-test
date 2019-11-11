#!/bin/bash
#
#    Authors:
#        Pavel Březina <pbrezina@redhat.com>
#
#    Copyright (C) 2016 Red Hat
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# Test selected pam stack against authentication.
#
# Usage: pam_test.sh stack username password
#

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
