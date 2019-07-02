#ifndef __LED_H__
#define	__LED_H__

#define	LED_MAGIC	'L'
#define	LED_MAXNR	6

typedef struct {
	unsigned int size;
	char buff[128];
} __attribute__((packed))LED_INFO;

#define	LED_OFF			_________
#define	LED_ON			_________
#define	LED_GETSTATE	_IO(LED_MAGIC, 2)

#define	LED_READ	_IOR(LED_MAGIC, 3, LED_INFO)
#define	LED_WRITE	_IOW(LED_MAGIC, 4, LED_INFO)
#define	LED_RW		_IOWR(LED_MAGIC, 5, LED_INFO)

#endif
