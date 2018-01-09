# remove some soft links for the shared object files


# for x86

cd ../../gnu/zlib/lib/linux/x86

rm libz.so.1
rm libz.so

cd -
cd ../../gnu/openssl/lib/linux/x86

rm libcrypto.so
rm libssl.so

cd -
cd ../../gnu/curl/lib/linux/x86

rm libcurl.so.4
rm libcurl.so


# for x64

cd ../../gnu/zlib/lib/linux/x64

rm libz.so.1
rm libz.so

cd -
cd ../../gnu/openssl/lib/linux/x64

rm libcrypto.so
rm libssl.so

cd -
cd ../../gnu/curl/lib/linux/x64

rm libcurl.so.4
rm libcurl.so
