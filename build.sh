gcc -static -Os -DLAST hello.c -o hellolast -ffreestanding -nostdlib
gcc -static -Os  -ffreestanding -nostdlib -I./fslibc/include -L./fslibc -o hellofslc fsl-init.c hello.c -lfslc

diet gcc -static -Os hello.c -o hellodiet 2> /dev/null
musl-gcc -static -Os hello.c -o hellomusl
gcc -static -Os hello.c -o hellogcc

zig build-exe -O ReleaseSmall -static --name helloziggcc  -target x86_64-linux-gnu hello.zig
zig build-exe -O ReleaseSmall -static --name hellozigmusl -target x86_64-linux-musl hello.zig

nasm -f elf64 -o helloasm.o hello.s
ld -o helloasm helloasm.o
trash helloasm.o

exa -lBhS --no-time --no-user -s size | grep rwxrwx | sed -e 's/\.rwxrwxr-x//g'

echo
size -G helloasm hellolast hellofslc hellodiet hellomusl hellozigmusl helloziggcc hellogcc 

echo

for f in * ; do
  if [ -x "$f" ] && file "$f" | grep -q "ELF"
  then
    echo -n "$f "
    ./$f
  fi
done
