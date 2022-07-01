<?php
require "funcs.php";
if($argc < 3)
    exit("nao informado");

$numero1 = preg_replace("/[^0-9]/", "", $argv[1]);
$numero2 = preg_replace("/[^0-9]/", "", $argv[2]);
$result = sumlong($numero1, $numero2);
echo $result."\n";
for($i = 3; $i < $argc; $i++){
    
    $numero1 = $result;
    $numero2 = $argv[$i];
    $result = sumlong($numero1, $numero2);
    echo $result."\n"; 
}

$arq = fopen("lognum.log", "a");
fwrite($arq, $result."\r\n");
fclose($arq);
?>