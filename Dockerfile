FROM alpine:3.18 AS build
RUN apk update && \
    apk add --no-cache \
    build-base \
    cmake \
    asio-dev \
    sqlite-dev \
    libcrypto3

WORKDIR /wzcalendar
COPY src/ ./src/
COPY libs/ ./libs/
COPY CMakeLists.txt ./
COPY run_build.sh ./
RUN ./run_build.sh

ENTRYPOINT ["/wzcalendar/dist/wzcalendar"]

# docker run --name wzcalendar -v $PWD/db/:/mnt/ --network host -t -d wzcalendar:latest