#php extension for utils
C++ extension for PHP 

## Prerequisites
  1. <sub>PHP 5.3 or higher</sub>
  2. <sub>c++0x or higher (g++ 4.4.6 or higher)</sub>
  3. <sub>Make sure you've *phpize* and *php-config* installed.</sub>

## Installation
  1. <sub>cd php_extension_utils</sub>
  2. <sub>/path/to/phpize</sub>
  3. <sub>./configure CXXFLAGS=-std=gnu++11</sub>
  4. <sub>make</sub>
  5. <sub>sudo make install</sub>

> Add *extension=utils.so* to your php.ini

>  running  **/path/to/php -d"extension=utils.so" utils.php**

