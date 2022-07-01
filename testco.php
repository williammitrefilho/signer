<?php
$pre_master_secret = file_get_contents("key_material/pre_master_secret.bn");
//PRF para o master secret
$label = "extended master secret";
$transcript = file_get_contents("transcripts/transcript-t.bn");
$transcript_hash = hash("sha384", $transcript, true);
$seed = $transcript_hash;
$secret = $pre_master_secret;

$a1 = hash_hmac("sha384", $label.$seed, $secret, true);
$master_secret = hash_hmac("sha384", $a1.$label.$seed, $secret, true);// PRF(secret, label, seed) = P_Hash(secret, A[i] + (label + seeed));
echo "master secret:\n".chunk_split(bin2hex($master_secret), 32)."\n";
$master_secret_b = hash_hkdf("sha384", $secret, 48, $label, $seed);
echo "master secret b:\n".chunk_split(bin2hex($master_secret_b), 32)."\n";

//PRF para o key material
$secret = $master_secret;
$server_client_random = file_get_contents("key_material/server_client_random.bn");
$seed = $server_client_random;
$label = "key expansion";
$key_material = "";
$a1 = hash_hmac("sha384", $label.$seed, $secret, true);
for($i = 0; $i < 4; $i++){
    
    $key_material .= hash_hmac("sha384", $a1.$label.$seed, $secret, true);// PRF(secret, label, seed) = P_Hash(secret, A[i] + label + seeed);
    $a1 = hash_hmac("sha384", $a1, $secret, true);
}
//echo "key material:\n".chunk_split(bin2hex($key_material), 32)."\n";

//PRF para o verify_data
$secret = $master_secret;
$label = "client finished";
$seed = $transcript_hash;

$a1 = hash_hmac("sha384", $label.$seed, $secret, true);
//echo "initial state(".strlen($a1.$label.$seed)."):\n".chunk_split(bin2hex($a1.$label.$seed), 32)."\n";
$verify_data = hash_hmac("sha384", $a1.$label.$seed, $secret, true);// PRF(secret, label, seed) = P_Hash(secret, A[i] + label + seeed);
//echo "verify_block:\n".chunk_split(bin2hex($verify_data), 32)."\n";
$verify_data = substr($verify_data, 0, 12);
echo "verify_data:\n".bin2hex($verify_data)."\n";

$tstate = file_get_contents("tstate-client_finished");
if($tstate != $a1.$label.$seed){
    
    $state = $a1.$label.$seed;
    for($i = 0; $i < strlen($tstate); $i++){
        
        if($tstate[$i] != $state[$i]){
            
            echo "diferenca(".$i."):".$tstate[$i]." - ".$state[$i];
            break;
        }
    }
}
$verify_tstate = hash_hmac("sha384", $tstate, $secret, true);
echo "verify_tblock:\n".chunk_split(bin2hex($verify_tstate), 32)."\n";
?>