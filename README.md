Lightning Cash Gold Core integration/staging tree
===========================================

<a href="https://imgbb.com/"><img src="https://i.ibb.co/RvyqxKN/bitcoin.png" alt="download" border="0"></a>

What is LightningCash-Gold ?
----------------------

Lightning Cash Gold is a coin based on scrypt POW algorithm, including DarkGravityWave ( difficulty changes for every block ) and possible CURRENT block difficulty change over time to protect against high network hash variation and long stale tip. For full details, please visit our website at http://lightningcash-gold.com . Compiled binary releases are available on LightningCash-Gold's github.

Lightning Cash Gold Core is the full node software that makes up the backbone of the LTNCG network.


Discord Channel : https://discord.gg/Q69zY4Y


bitcointalk.org official announcement thread : https://bitcointalk.org/index.php?topic=5119555




Characteristics
---------------------------------------------------------------------------


- Hash Algorithm : SCRYPT POW algorithm    ( Asic-friendly )

( With the addition of modified Dash's DarkGravityWave v3 algorithm, that changes difficulty every block, and progressively lowers difficulty if long stale tips happens. Tested and fully working !!)

- The Hive : A " hodl and profit " mining system accessible to everyone, that works side by side with POW mining, invented by  LitecoinCash's developers !!

- Total Supply : 2 100 000 000 LTNCG ( 100 times Bitcoin's total supply )

- Difficulty adjusted every block and possibly in CURRENT block if a long network stale tip happens !! This protects LightningCash-Gold from high hash variations and " high hash attacks " , that happens frequently for new coins and kills them by making the mining difficulty too high for the " normal " network's hash rate. For more info :
		https://bitcointalk.org/index.php?topic=5119550 .

- Changed famous NOMP ( node open mining portal ) pool software to include possible CURRENT block difficulty change and send new work to miners if so.

NOMP's node-stratum-pool module adapted code : 

		https://github.com/Lightningcash-dev/node-stratum-pool

- Fast POW / Hive block pair target time of 1 minute !! 

- Number of confirmations needed for a transaction : 6  ( so a transaction is confirmed on average in 3 minutes only !!! )

- Premine : 22 000 000 coins, barely above 1 % of max supply

- Block Reward : 500 LightningCash ( halved every 2 100 000 blocks, so every 4 years on average )

- Brand New Blockchain !!  ( downloading the new blockchain or re indexing it and syncing with the network actually takes less than 1 minute on average with a normal internet speed !!! )


Official Website :

http://lightningcash-gold.com



Official Mining Pool :

http://pool.lightningcash-gold.com/



Official Block Explorer :

http://explorer.lightningcash-gold.com/




White Paper available on LightningCash-Gold's website.



Exchange : 

https://fcbaccount.com/
https://swiftex.co/
https://probitex.com/
https://enmanet.com/

Other exchanges are under discussions.


LightningCash Gold is based on LitecoinCash Core's open source code.
LightningCash Gold is an independant coin and is not associated in any way with LitecoinCash.


License
-------

Lightning Cash Gold Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

You can fork LitecoinCash-Gold github's repository, tweak the code and suggest pull request if you have bug fixes or improvements to propose.

Testing
-------

For now, testing is done privately before releasing new wallet's versions.

Translations
------------

Any translation corrections or expansions are welcomed as GitHub pull requests.
