FROM ubuntu:24.04 AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY main.cc ./
RUN g++ -D_GNU_SOURCE -std=c++17 -Wall -Wextra -O2 -static main.cc -o /app/app

FROM ubuntu:24.04

COPY --from=build /app/app /usr/local/bin/app
COPY itachi/ /itachi/
COPY sasuke/ /sasuke/
ENTRYPOINT ["/usr/local/bin/app"]
