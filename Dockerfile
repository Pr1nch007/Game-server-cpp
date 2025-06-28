FROM gcc:11.3 as build

RUN apt update && \
    apt install -y \
      python3-pip \
      cmake \
    && \
    pip3 install conan==1.*

COPY conanfile.txt /app/
RUN mkdir /app/build && cd /app/build && \
    conan install .. --build=missing -s build_type=Release -s compiler.libcxx=libstdc++11

COPY ./src /app/src
COPY ./tests /app/tests
COPY CMakeLists.txt /app/

RUN cd /app/build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build .

FROM ubuntu:22.04 as run

RUN groupadd -r www && useradd -r -g www www
WORKDIR /app

RUN mkdir -p /tmp/volume && chown www:www /tmp/volume

COPY --from=build /app/build/game_server /app/
COPY --from=build /app/build/game_server_tests /app/
COPY ./data /app/data
COPY ./static /app/static

RUN chown -R www:www /app

USER www
  
ENTRYPOINT ["./game_server", \
            "--config-file", "./data/config.json", \
            "--www-root", "./static"]