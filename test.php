<?php
print_r(stream_get_transports());
$sck = fsockopen("tlsv1.2://gerio.app", 443, $errno, $errstr);
if(!$sck){
    
    exit("erro:".$errstr);
}
fwrite($sck, "GET / HTTP/1.1\r\nHost:gerio.app\r\n\r\n");
$linha = fgets($sck);
echo $linha;
fclose($sck);
?>