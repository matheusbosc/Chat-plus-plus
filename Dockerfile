FROM ubuntu:latest
LABEL authors="Matheus Boscariol"

WORKDIR /

COPY bin/server /

CMD ["./server"]