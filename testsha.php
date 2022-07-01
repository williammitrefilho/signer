<?php
$teste = "linha pequenina";
$teste1 = "linha grandeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";

$hash = hash("sha384", $teste);
$hash1 = hash("sha384", $teste1);

echo $hash."\n".$hash1."\n\n";

$chave = file_get_contents("key_material/master_secret.bn");

$mac = hash_hmac("sha384", $teste, $chave);
$mac1 = hash_hmac("sha384", $teste1, $chave);

echo $mac."\n".$mac1;
?>