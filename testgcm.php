<?php
$msg = file_get_contents("key_material/ch.bn");
$keys = file_get_contents("key_material/key_block.bn");
$ad = file_get_contents("key_material/ad.bn");
$ad1 = hex2bin("1111111111111111");
$ad2 = hex2bin("000000000000000117030301D9");

$message = "GET / HTTP/1.1\r\nHost: gerio.app\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.82 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9,pt-BR;q=0.8,pt;q=0.7\r\n\r\n";

$ad2 = hex2bin("0000000000000001170303".sprintf("%04X", strlen($message)));

$nonce = file_get_contents("key_material/nonce.bn");

$client_write_key = substr($keys, 0, 32);
$client_write_iv = substr($keys, 64, 4);


$iv = $client_write_iv.$nonce;

$ciphered = openssl_encrypt($msg, "aes-256-gcm", $client_write_key, OPENSSL_RAW_DATA | OPENSSL_ZERO_PADDING, $iv, $tag, $ad);
echo bin2hex($ciphered)."\n";
echo bin2hex($tag)."\n";

$deciphered = openssl_decrypt($ciphered, "aes-256-gcm", $client_write_key, OPENSSL_RAW_DATA | OPENSSL_ZERO_PADDING, $iv, $tag, $ad);
echo bin2hex($deciphered)."\n\n";

$nonce[7] = chr(ord($nonce[7]) + 1);
$iv = $client_write_iv.$nonce;

$ciphered = openssl_encrypt($message, "aes-256-gcm", $client_write_key, OPENSSL_RAW_DATA | OPENSSL_ZERO_PADDING, $iv, $tag2, $ad2);

echo bin2hex($tag2)."\n";
echo bin2hex($ad2)."\n";
echo bin2hex($iv)."\n";
echo bin2hex($ciphered)."\n";

$deciphered = openssl_decrypt($ciphered, "aes-256-gcm", $client_write_key, OPENSSL_RAW_DATA | OPENSSL_ZERO_PADDING, $iv, $tag2, $ad2);

echo $deciphered."(".strlen($message).")\n";
echo bin2hex($tag2)."\n";
echo bin2hex($ad2)."\n";
echo bin2hex($iv)."\n";
?>