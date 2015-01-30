#!/bin/sh

if test $# != 1 ; then 
    exit 1
fi

HEAD=`git symbolic-ref HEAD`

echo -n $*.

if test ${HEAD} != "refs/heads/master" ; then
    echo -n ${HEAD}. | tr '/' '.'
fi

git log --oneline | wc -l | tr -d '\n'
