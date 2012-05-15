#!/usr/bin/env sh

export BOTNAME=bot
dbus-launch --config-file=/etc/dbus-1/session.conf --exit-with-session src/tp_bot

