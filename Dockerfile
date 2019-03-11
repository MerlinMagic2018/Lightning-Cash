FROM ubuntu:18.04

RUN apt-get update

RUN apt-get install -y build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 libboost-all-dev

RUN apt-get install -y software-properties-common

RUN add-apt-repository -y ppa:bitcoin/bitcoin

RUN apt-get update

RUN apt-get install -y libdb4.8-dev libdb4.8++-dev

RUN apt-get install -y libminiupnpc-dev

RUN apt-get install -y libzmq3-dev

RUN apt-get install -y git

RUN git clone https://github.com/Lightningcash-dev/lightningcash-gold

WORKDIR /lightningcash-gold

RUN ./autogen.sh

RUN ./configure --without-gui --disable-tests --disable-bench

RUN make

RUN make install

RUN mkdir /root/.lightningcash_gold

RUN cp lightningcash_gold.conf /root/.lightningcash_gold/lightningcash_gold.conf

EXPOSE 9111 9110
