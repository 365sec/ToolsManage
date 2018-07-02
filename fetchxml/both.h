#pragma once
#pragma warning(disable:4200)


#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <process.h>
#else
	#include <errno.h>
	#include <string.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>

	#include <pthread.h>
	#include <unistd.h>
	#include<sys/stat.h>
	#include<fcntl.h>
#endif


//����----�����̣�һ��Ҫע��˳��
//gcc -std=c++11 -lstdc++ -lpthread -I/usr/local/openssl/include CBCdes.cpp /usr/local/openssl/lib/libcrypto.a -ldl Collect.cpp /opt/zlib/lib/libz.a grwifi.cpp -o wifimgr
#ifndef uint_16
#define uint_16 unsigned short
#endif

#ifndef uint_32
#define uint_16 unsigned short
#endif

#define ADD(x,y)			(x+y)


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef Add2Ptr
#define Add2Ptr(P,I) ((void*)((unsigned char*)(P) + (I)))
#endif

	#define myread(x,y,z)	recv(x,y,z,0)

#ifdef _WIN32
	#define	MyInitCsMutex(x)	InitializeCriticalSection(&x)
	#define	MY_CS_MUTEX	CRITICAL_SECTION
	#define MY_LOCK		EnterCriticalSection	
	#define MY_UNLOCK	LeaveCriticalSection	
	#define MyDeleteMutex DeleteCriticalSection

	#define THREAD_RET	void
	#define	DO_RET
	#define	MY_BEGINTHREAD(x,y,z)	z=_beginthread((x),0,(y))
	#define MY_WAITFOR(x)			WaitForSingleObject((HANDLE)x,INFINITE)
	#define	MYTIMEOUT(x)			(x*1000)	//��λ����


	#define bzero(x,y)		memset(x,0,y)
	#define socklen_t		int
	#define	MyCloseSocket(x)	closesocket(x)
	#define	MyShutDown(x)		closesocket(x)
	#define MySleepSecond(x)		Sleep(x*1000)		//��λ����
	#define THREADID		unsigned long
	#define MySend(x,y,z)	send(x,y,z,0)
	#define Myperror(x)		printf(x" error=%d\n",GetLastError())
	#define	MyINT64			__int64
	#define	STRLL			"%I64d"

	#define MyGetTID()		GetCurrentThreadId()
	#define MyGetPID()		GetCurrentProcessId()
	#define my_stricmp		stricmp
#else
	#define	MyInitCsMutex(x)	(x=PTHREAD_MUTEX_INITIALIZER)
	#define	MY_CS_MUTEX	pthread_mutex_t
	#define MY_LOCK		pthread_mutex_lock	
	#define MY_UNLOCK	pthread_mutex_unlock	
	#define MyDeleteMutex pthread_mutex_destroy

	#define THREAD_RET	void*
	#define	DO_RET		return((THREAD_RET)0)
	#define	MY_BEGINTHREAD(x,y,z)	pthread_create(&z,NULL,(x),(y))	
	#define MY_WAITFOR(x)			pthread_join(x,NULL)
	#define	MYTIMEOUT(x)			(x)		//��λ��


	#define	MyCloseSocket(x)	close(x)
	#define	MyShutDown(x)		shutdown(x,2)	//how=0 ��ֹ��ȡ������how=1 ��ֹ���Ͳ��� how=2 ��ֹ��ȡ�����Ͳ���
	#define MySleepSecond(x)		sleep(x)		//��λ΢��
	#define THREADID		pthread_t
	#define MySend(x,y,z)	write(x,y,z)
	#define Myperror(x)		perror(x)
	#define	MyINT64			long long
	#define	STRLL			"%lld"

	#define MyGetTID()		gettid()
	#define MyGetPID()		getpid()
	#define my_stricmp		strcasecmp
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1 
#endif

#ifndef FALSE
#define FALSE 0
#endif

//qizc
#ifndef UINT16
typedef unsigned short UINT16;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef ULONG32
typedef unsigned int ULONG32;
//#define ULONG32 unsigned long
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#ifndef INT32
typedef int INT32;
#endif

#ifndef UINT32
typedef unsigned int UINT32;
#endif

#ifndef MAX_PATH
#define MAX_PATH (260*2)
#endif

#ifndef INT
typedef int INT;
#endif

#ifndef VOID
typedef void VOID;
#endif

#ifndef LONG
typedef long LONG;
#endif


//
//gcc -lstdc++ -lpthread Collect.cpp grwifi.cpp -o wifimgr
//gcc++ -lpthread Collect.cpp grwifi.cpp -o wifimgr
//
//	����������������о���
//  extended initializer lists only available with -std=c++11 or -std=gnu++11 [Ĭ������]
//
//
//gcc -std=c++11 -lstdc++ -lpthread Collect.cpp grwifi.cpp -o wifimgr
//gcc -std=gnu++11 -lstdc++ -lpthread Collect.cpp grwifi.cpp -o wifimgr
//

//����----����----openssl
//gcc -I/usr/local/openssl/include destest.cpp /usr/local/openssl/lib/libcrypto.a -ldl -o destest

//����----����----zip
//gcc  1.cpp /opt/zlib/lib/libz.a -o 1

//����----�����̣�һ��Ҫע��˳��
//gcc -std=c++11 -lstdc++ -lpthread -I/usr/local/openssl/include CBCdes.cpp /usr/local/openssl/lib/libcrypto.a -ldl Collect.cpp /opt/zlib/lib/libz.a grwifi.cpp -o wifimgr


/*
����װ���������64λCentOS�� /lib64/libc.so.6: version `GLIBC_2.14' not found
	�鿴��ǰ glibc �汾��rpm -qa |grep glibc
	�鿴��ǰ glibc �汾��strings /lib64/libc.so.6 |grep GLIBC_
	Ϊʲô��Ҫ�鿴������Ѿ���װ������Ҫ���û�����


	[root@localhost ~]# tar xvf glibc-2.14.tar.gz
	[root@localhost ~]# cd glibc-2.14
	[root@localhost glibc-2.14]# mkdir build
	[root@localhost glibc-2.14]# cd ./build
	[root@localhost build]# ../configure --prefix=/lib64/glibc-2.14
	[root@localhost build]# make -j4
	[root@localhost build]# make install
	
������Ѿ���װ��
	//������һ�����Բ���Ҫ
	[root@localhost ���·��]��������һ��
	# export LD_LIBRARY_PATH=/lib64/glibc-2.14/lib:$LD_LIBRARY_PATH
	# export LD_LIBRARY_PATH=/opt/glibc-2.14/lib:$LD_LIBRARY_PATH
*/

/*

  //����������ӵ� /etc/profile	�÷���
  //vi profile---������������һ����ĸ----�����ַ����л�Ϊinsert״̬-----ճ��-----ESC----:----wq
  export LD_LIBRARY_PATH=/opt/glibc-2.14/lib:$LD_LIBRARY_PATH
  TZ='Asia/Shanghai'; export TZ
*/

/*
��û��Ȩ��ִ�С�
	�鿴���ļ�Ȩ�ޣ�ll 
	�޸�Ȩ�ޣ�chmod ��mode�� �ļ���
	�ٸ����ӣ�chmod a+x grwifi
		  ./grwifi
*/

/*
һ��Linuxƽ̨

$wget http://www.zlib.net/zlib-1.2.3.tar.gz
$tar -xvzf zlib-1.2.3.tar.gz
$cd zlib-1.2.3.tar.gz
$./configure --prefix=/opt/zlib ���߾��� $./configure
$make
$sudo make install

�������ȿ����Լ��Ļ������ǲ����Ѿ���װ��zlib�ˣ�

����whereis zlib

���������װ���ˣ������zlib��·��������ֱ������ǰ2����

����1.��������zlib�����°棬�ҵ���1.2.3
����2.������ѹ�����룺

  ����./configure --prefix=/opt/zlib
	����make
	����sudo make install

����3.����zlib��װ���ˣ���������дһ���������һ�£�

  ���롣ע�� -lz ˳���ں�

    gcc -Wall -o 1 1.cpp -lz
	gcc  1.cpp /opt/zlib/lib/libz.a -o 1

clean:   
    rm -rf *.o test  
����ע�⵽��������-lz������zlib��

����test.c

����Windows ƽ̨��
	zlib-1.2.3\projects\visualc6\zlib.dsw ѡ�� Win32_LIB_Release ���ɾ�̬�� ��
		����� zlib.h/zconf.h

*/


//lib�ļ�--���·��������linux�������ò��ã���ʧ�� 
//gcc -I/usr/local/openssl/include destest.cpp -lcrypto -ldl -o destest

//lib�ļ�--����·��
//gcc -I/usr/local/openssl/include destest.cpp /usr/local/openssl/lib/libcrypto.a -ldl -o destest


/*
1�����ص�ַ��http://www.openssl.org/source/ ��һ���°汾��OpenSSL�����µİ汾�ǣ�openssl-1.0.0e.tar.gz
2�������ص�GZĿ¼�У�������ִ�У�tar -xzf openssl-1.0.0e.tar.gz
3�������ѹ��Ŀ¼��openssl-1.0.0e  [.......]#cd openssl-1.0.0e
4��[.....openssl-1.0.0e]# ./config --prefix=/usr/local/openssl
5��[...../openssl-1.0.0e]# ./config -t
6��[...../openssl-1.0.0e]# make depend
7��[...../openssl-1.0.0e]# cd /usr/local
8��/usr/local]# ln -s openssl ssl
9����/etc/ld.so.conf�ļ�������棬����������ݣ������⻰vi���� ȱʡ������ģʽ��i�����޸�ģʽ--->esc--->wq��
	/usr/local/openssl/lib
10��...]# ldconfig
11�����OPESSL�Ļ���������
	��etc����profile�����һ�У���ӣ�
	export OPENSSL=/usr/local/openssl/bin
	export PATH=$OPENSSL:$PATH:$HOME/bin
12���˳�������棬�ٴ��µ�¼��
13������OPENSSL�Ͱ�װ��ϣ��������һЩ��顣
14����������ִ�У�
[root@localhost /]# cd /usr/local
[root@localhost local]# ldd /usr/local/openssl/bin/openssl
	���������������Ϣ��
    	linux-vdso.so.1 =>  (0x00007fff3bc73000)
    	libdl.so.2 => /lib64/libdl.so.2 (0x00007fc5385d7000)
    	libc.so.6 => /lib64/libc.so.6 (0x00007fc538279000)
    	/lib64/ld-linux-x86-64.so.2 (0x00007fc5387db000)

15���鿴·��
...]# which openssl
	/usr/local/openssl/bin/openssl

16���鿴�汾
...]# openssl version
	OpenSSL 1.0.0e 6 Sep 2011
*/
