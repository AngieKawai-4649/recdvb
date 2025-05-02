#【ビルド方法】
# 1.STARBUCKSを使用
#    1.1 libarib25をセットアップする
#    1.2 libstarbucksをセットアップする
#    1.3 recdvb/srcディレクトリで
#        make STARBUCKS=1
#        sudo make install (recdvbを/usr/local/binにコピー)
#
# 2.外付けカードリーダーを使用
#    2.1 recdvb/srcディレクトリで
#        make CARDREADER=1
#        sudo make install
#
# 3.B25解除を行わない場合
#    B25解除を当プログラムで行わず、mirakurun arib-b25-stream-test等で行う場合
#    recdvb/srcに移動し
#    make
#    sudo make install
#    注 1or2でビルドした物で--b25オプションを付けなければ同様の処理を実現できる
#
# B25処理を変更する為にビルドし直す場合はフルコンパイルする
# 例：外付けカードリーダーからSTARBUCKSに変更する場合
# make CARDREADER=1 clean
# make STARBUCKS=1
#
#
# それぞれのbuildによりリンクするカードリーダーライブラリが異なるので確認する
# ldd recdvb
#
# STARBUCKS  : libarib25.so.0直下にlibstarbucks.so
# CARDREADER : libarib25.so.0直下にlibpcsclite.so.1

ifdef STARBUCKS
#STD_B25 = -DSTD_B25
HAVE_LIBARIB25 = -DHAVE_LIBARIB25
LIBS    =  -larib25 -lstarbucks
else
ifdef CARDREADER
HAVE_LIBARIB25 = -DHAVE_LIBARIB25
LIBS    =  -larib25 `pkg-config libpcsclite --libs`
endif
endif

OBJSC	= recdvb.o decoder.o mkpath.o time.o recdvbcore.o queue.o reader.o

CHANNELCNF = `pkg-config libchannelcnf --cflags`

CXX	= g++
CC	= gcc
CFLAGS   = -O2 -Wall -pthread -D_LARGEFILE_SOURCE $(HAVE_LIBARIB25) $(CHANNELCNF)
ifeq ($(shell getconf LONG_BIT),32)
CFLAGS  += -D_FILE_OFFSET_BITS=64
endif

LIBS	+= -lpthread `pkg-config libchannelcnf --libs`

PREFIX = /usr/local
TARGET	= recdvb

all: $(TARGET)

clean:
	rm -f $(OBJSC) $(TARGET)

uninstall:
	rm -vf $(PREFIX)/bin/$(TARGET)

install: uninstall
	install -m 755 $(TARGET) $(PREFIX)/bin

$(TARGET): $(OBJSC)
	$(CXX) -o $(TARGET) $(OBJSC) $(LIBS)
ifdef STARBUCKS
	@echo "\n"B25 DECODE USING STARBUCKS"\n"
else
ifdef CARDREADER
	@echo "\n"B25 DECODE USING CARDREADER"\n"
endif
endif
ifndef HAVE_LIBARIB25
	@echo "\n"B25 DECODE OFF
	@echo "STARBUCKS    -> make STARBUCKS=1"
	@echo "CARDREADER -> make CARDREADER=1""\n"
endif


depend:
	$(CXX) -MM $(OBJSC:.o=.c) >> Makefile.dep

-include Makefile.dep

