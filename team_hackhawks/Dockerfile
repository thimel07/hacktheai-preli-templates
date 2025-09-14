FROM alpine:latest

WORKDIR /app

RUN apk add --no-cache gcc musl-dev

COPY main.c .

RUN gcc -O2 -o server main.c

EXPOSE 8000

CMD ["./server"]
