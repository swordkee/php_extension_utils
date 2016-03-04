<?php
if(!extension_loaded('utils')) {
	dl('utils.' . PHP_SHLIB_SUFFIX);
}
$uuid = new uuid();
$uuid->setEpoch(1456814586460);
$uuid->setMachine(100);


for ($i = 0; $i < 10; ++$i) {
    print $uuid->getGenerate()."\n";
}


?>
