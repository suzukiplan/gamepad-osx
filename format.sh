#!/bin/sh
clang-format -assume-filename=.clang-format <$1 >.work
diff .work $1 >/dev/null
if [ $? -eq 1 ]
then
	cat .work > $1
fi
\rm -f .work
