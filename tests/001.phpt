--TEST--
Check for utils presence
--SKIPIF--
<?php if (!extension_loaded("utils")) print "skip"; ?>
--FILE--
<?php

$uuid = new uuid();
$uuid->setEpoch(1456814586460);
$uuid->setMachine(100);

for ($i = 0; $i < 10; ++$i) {
    print $uuid->getGenerate()."\n";
}

echo "utils extension is available";
?>
--EXPECT--
utils extension is available
