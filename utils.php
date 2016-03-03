<?php
if(!extension_loaded('utils')) {
	dl('utils.' . PHP_SHLIB_SUFFIX);
}
$uuid = new uuid();
var_dump(msectime())."\n";
$uuid->setEpoch(msectime());
$uuid->setMachine(100);


for ($i = 0; $i < 10; ++$i) {
    print $uuid->getGenerate()."\n";
}

function msectime() {
    list($tmp1, $tmp2) = explode(' ', microtime());
    return (float)sprintf('%.0f', (floatval($tmp1) + floatval($tmp2)) * 1000);
}

?>
