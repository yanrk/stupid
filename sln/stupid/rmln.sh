# remove some soft links for the shared object files

cd ../../gnu/zlib/lib/linux

rm libz.so.1
rm libz.so

cd -
cd ../../gnu/openssl/lib/linux

rm libcrypto.so
rm libssl.so

cd -
cd ../../gnu/curl/lib/linux

rm libcurl.so.4
rm libcurl.so
