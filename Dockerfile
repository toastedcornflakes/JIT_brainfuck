FROM debian 

RUN apt-get update && apt-get install -y build-essential libc6 strace

WORKDIR /var/src/jit
ADD . .

ENTRYPOINT make clean && make && ./brain scripts/helloworld.fck

