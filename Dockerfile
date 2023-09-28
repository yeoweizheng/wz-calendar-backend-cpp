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
COPY build.sh ./
RUN ./build.sh

FROM alpine:3.18
RUN apk update && \
    apk add --no-cache \
    asio-dev \
    sqlite-dev \
    libcrypto3

COPY --from=build /wzcalendar/dist/wzcalendar /wzcalendar

ENTRYPOINT ["/wzcalendar"]

# docker run --name wzcalendar -v $PWD/db/:/mnt/ --network host -t -d wzcalendar:latest