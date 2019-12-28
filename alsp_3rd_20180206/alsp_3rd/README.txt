Advanced Linux System & Network Programming 3rd Edition (2016) 예제 모음
- version : 2017년 6월 3일 토요일
- version : Sat Jun 3 2017

* 옵션 패키지 (RPM 기준: 페도라/레드햇/센트OS 인 경우)
pcre2-devel

만일 Debian계열을 사용한다면 libpcre2-dev libpcre2-posix0 를 설치한다.

pcre2 기능이 없어도 관련 기능을 제외하고 빌드 됩니다.
따라서 pcre2가 지원되지 않는 시스템에서도 빌드가 가능합니다.

-------------------------------------------------------------------------------
* 컴파일 방법
0) pcre2-devel 패키지가 설치되어있지 않다면 설치한다.(지원되지 않는 시스템이면 건너뛰어도 된다.)
1) 예제 파일의 압축을 푼다.
2) alsp_3rd 디렉터리로 이동한다.
3) make 를 실행하면 모두 컴파일이 된다.
   (각각의 디렉토리에서 개별 컴파일 할 때는 각 챕터별 서브 디렉토리에서 make를 명령하면 됩니다.)

-------------------------------------------------------------------------------
* 디렉토리 구조
alsp_3rd
  |
  |-- include : 헤더 디렉토리
  |-- lib     : 사용자 라이브러리 디렉토리
  +-- src
       |
	   |-- 0.preface         : Chapter  0. Preface 예제
	   |-- 1.process         : Chapter  1. Process 예제
	   |-- 2.file            : Chapter  2. File 예제
       |-- 3.text            : Chapter  3. Text 예제
	   |-- 4.memory          : Chapter  4. Memory 예제
	   |-- 5.ipc             : Chapter  5. IPC(Inter Process Communication) 예제
	   |-- 6.io_interface    : Chapter  6. I/O Interface 예제
       |-- 7.io_multiplexing : Chapter  7. I/O Multiplexing 예제
	   |-- 8.pthread         : Chapter  8. POSIX Thread / OpenMP 예제
	   |-- 9.signal          : Chapter  9. Signal 예제
	   |-- 10.realtime       : Chapter 10. Realtime extentions 예제
	   +-- 11.linux_specific : Chapter 11. Linux Specific 예제

===============================================================================
* Optional package (for RPM based : Fedora/RedHat/CentOS)
pcre2-devel

If you are using Debian or Ubuntu, install libpcre2-dev and libpcre2-posix0.

-------------------------------------------------------------------------------
* Compiling
0) Install pcre2-devel package if not installed on your linux host. (optional)
1) Extract the ALSP 3rd edition source file.
2) Change directory to alsp_3rd.
3) Run the following command to build all examples.
   make

-------------------------------------------------------------------------------
* Source Directory

alsp_3rd
  |
  |-- include : user header
  |-- lib     : user library
  +-- src
       |
	   |-- 0.preface         : Chapter  0. Preface : examples
	   |-- 1.process         : Chapter  1. Process : examples
	   |-- 2.file            : Chapter  2. File : examples
           |-- 3.text            : Chapter  3. Text : examples
	   |-- 4.memory          : Chapter  4. Memory : examples
	   |-- 5.ipc             : Chapter  5. IPC(Inter Process Communication) : examples
	   |-- 6.io_interface    : Chapter  6. I/O Interface : examples
           |-- 7.io_multiplexing : Chapter  7. I/O Multiplexing : examples
	   |-- 8.pthread         : Chapter  8. POSIX Thread & OpenMP : examples
	   |-- 9.signal          : Chapter  9. Signal : examples
	   |-- 10.realtime       : Chapter 10. Realtime extentions : examples
	   +-- 11.linux_specific : Chapter 11. Linux Specific : examples


-------------------------------------------------------------------------------
* Chgange history

* 2018.01.02
코드 정리

* 2017.06.03
코드 정리

* 2017.05.29
Chapter 10.
    pthread_pi_cputime_sched.c 추가
    sched_timequantum.c 추가
    watch_process.sh 추가

* 2016.08.19 (박강민님의 제보로 수정된 부분입니다.)
Chapter 5.
    posix_nameless_sem.c
        36행 : sem_init 리턴값 검사 코드 오류 수정
    lib_sysv_sem.c
        153행: int sysv_sempost(int sem_id, int sem_idx)함수 코드 부분에 SEM_UNDO플래그 추가.
Chapter 6.
    fifo_write.c
        38행 : 개행문자 삭제 부분 오류 수정
    fifo_write_sigpipe.c
        52행 : 개행문자 삭제 부분 오류 수정
    inet_tcp_serv1.c
        123행: 개행문자가 입력된 경우 출력을 예쁘게 하기 위한 코드 수정
        원문 : pr_out("[Child:%d] RECV(%.*s)", idx, ret_len, buf);
        변경 : pr_out("[Child:%d] RECV(%.*s)", idx, buf[ret_len-1] == '\n' ? ret_len-1 : ret_len, buf);
    inet6_tcp_serv1.c
        168행: 개행문자가 입력된 경우 출력을 예쁘게 하기 위한 코드 수정
        원문 : pr_out("[Child:%d] RECV(%.*s)", idx, ret_len, buf);
        변경 : pr_out("[Child:%d] RECV(%.*s)", idx, buf[ret_len-1] == '\n' ? ret_len-1 : ret_len, buf);

Chapter 7.
    io_epoll.c  io_epoll_trigger.c  io_poll.c  io_poll_LT.c  io_poll_nb.c  io_select.c  io_select2.c
        poll(),epoll_wait(),select()가 성공한 뒤 listener socket인 경우 accept() 후에 continue 하는 부분을 수정. 
        기존의 code도 논리적으로 오류는 없으나 성능상의 불이익이 있을 수 있기에 수정함.
        (continue가 없어지면서 else를 추가하여 loop의 구조가 약간 변경됨)
Chapter 11.
    io_signalfd_chld.c
        41번행: for 문의 i 초기화 누락 수정.
