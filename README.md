Lightning Cash Gold Core integration/staging tree
===========================================

<a href="https://imgbb.com/"><img src="https://i.ibb.co/RvyqxKN/bitcoin.png" alt="download" border="0"></a>

What is LightningCash-Gold ?
----------------------

Lightning Cash Gold is a coin based on Yespower POW algorithm, including DarkGravityWave ( difficulty changes for every block ) and possible CURRENT block difficulty change over time to protect against high network hash variation and long stale tip. For full details, please visit our website at https://lightningcash.gold . Compiled binary releases are available on LightningCash-Gold's github.

Lightning Cash Gold Core is the full node software that makes up the backbone of the LTNCG network.


Discord Channel : https://discord.gg/Q69zY4Y


bitcointalk.org official announcement thread : https://bitcointalk.org/index.php?topic=5161816




Characteristics
---------------------------------------------------------------------------


- Hash Algorithm : YespowerLTNCG POW algorithm    ( CPU only )

( With the addition of modified Dash's DarkGravityWave v3 algorithm, that changes difficulty every block, and progressively lowers difficulty if long stale tips happens. Tested and fully working !!)

- The Hive : A " hodl and profit " mining system accessible to everyone, that works side by side with POW mining, invented by  LitecoinCash's developers !!

- Total Supply : 210 000 000 LTNCG

- Difficulty adjusted every block and possibly in CURRENT block if a long network stale tip happens !! This protects LightningCash-Gold from high hash variations and " high hash attacks " , that happens frequently for new coins and kills them by making the mining difficulty too high for the " normal " network's hash rate. For more info :
		https://bitcointalk.org/index.php?topic=5119550 .

- Changed famous NOMP ( node open mining portal ) pool software to include possible CURRENT block difficulty change and send new work to miners if so.

NOMP's node-stratum-pool module adapted code : 

		https://github.com/Lightningcash-dev/node-stratum-pool

- 6.6 seconds block time.

- Number of confirmations needed for a transaction : 6, so a transaction is confirmed on average in 39.6 seconds only !!! 

- Premine : 2 200 000 coins, barely above 1 % of max supply

- Block Reward : 50 LightningCash - halved every 2 100 000 blocks

- Synchronising the wallet takes about 25 minutes with a 4 MB/s download speed.


Official Website :

https://lightningcashgold.com



Official Mining Pool :

http://pool.lightningcash.gold



Official Block Explorer :

http://explorer.lightningcash.gold




White Paper available on LightningCash-Gold's website.



Exchange : 

https://www.unnamed.exchange/MarketInfo/Coin/LTNCG

Other exchanges are continually under discussion.


LightningCash Gold is based on LitecoinCash Core's open source code.
LightningCash Gold is an independant coin and is not associated in any way with LitecoinCash.


License
-------

Lightning Cash Gold Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

The name " LightningCash-Gold " is copyrighted and belongs to Antoine Brule, LTNCG's founder.

Development Process
-------------------

You can fork LitecoinCash-Gold github's repository, tweak the code and suggest pull request if you have bug fixes or improvements to propose.

Testing
-------

For now, testing is done privately before releasing new wallet's versions.

Translations
------------

Any translation corrections or expansions are welcomed as GitHub pull requests.
