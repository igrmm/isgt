#!/bin/sh

cd src
nvim \
    -c "set makeprg=cd\ ..\ &&\ sh\ run.sh" \
    -c ":Telescope find_files" \
    -c "map <F5> :make<CR><CR>"
