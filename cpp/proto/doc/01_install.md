~~~shell
$ docker run -dit --name add_proto3 leizton/dev:0.7 /bin/bash
$ docker exec -it $CONTAINER_ID bash
$ cd tmp && wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.15.0-rc2.zip
$ unzip v3.15.0-rc2.zip
$ cd protobuf-3.15.0-rc2/
    cd protobuf/
    ./autogen.sh
    ./configure
    make
    sudo make install
    sudo ldconfig
$ protoc --version
$ cd /tmp && rm -rf protobuf-3.15.0-rc2 v3.15.0-rc2.zip
$ exit
$ docker commit -a 'leizton@126.com' -m 'add proto3' add_proto3 leizton/dev:0.8
$ docker push leizton/dev:0.8
~~~