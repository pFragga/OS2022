CC = cc
CFLAGS = -pthread

test-res:
	${CC} ${CFLAGS} 3200234-3200056-res.c -o test-res

clean:
	rm -vf test-res
