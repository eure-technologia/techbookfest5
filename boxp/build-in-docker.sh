#!/bin/bash

# コマンド手打ちで作業したい時は以下の通り /book に pwd がマウントされます
# docker run -i -t -v $(pwd):/book vvakame/review /bin/bash

docker build -t boxp/review .
docker run -t --rm -v $(pwd)/../:/book boxp/review /bin/bash -ci "cd /book/boxp && ./setup.sh && npm run pdf"
