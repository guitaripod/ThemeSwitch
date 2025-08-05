#!/bin/bash
# Simple theme toggle script for KDE Plasma

current=$(plasma-apply-colorscheme --list-schemes | grep "(current" | head -1)

if echo "$current" | grep -q "BreezeDark"; then
    plasma-apply-colorscheme BreezeLight
else
    plasma-apply-colorscheme BreezeDark
fi