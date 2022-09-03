<img src="https://i.ibb.co/3rvpw0v/68747470733a2f2f692e6962622e636f2f52767971784b4e2f626974636f696e2e706e67.png" alt="68747470733a2f2f692e6962622e636f2f52767971784b4e2f626974636f696e2e706e67" border="0"></a><br /><a target='_blank' href='https://imgbb.com/'></a><br />

# LightningCash Core integration/staging tree
===========================================
Updated on September 3rd 2022 

What is LightningCash ?
----------------------

LightningCash is a coin based on Yespower POW algorithm, including DarkGravityWave ( difficulty changes for every block ) and possible CURRENT block difficulty change over time to protect against high network hash variation and long stale tip. For full details, please visit our website at https://lightningcash-coin.com . Compiled binary releases are available here on Github.

Current supported version is 9.0.0 , 9.1.0 or 9.2.0

Lightning-Cash Core is the full node software that makes up the backbone of the LNC network.


Website : https://lightningcash-coin.com

Pool : https://pool.lightningcash-coin.com

Block Explorer : https://explorer.lightningcash-coin.com

Discord Server : https://discord.gg/WrMVXcA8bt


Exchange : https://unnamed.exchange ( Listing : Active status, but wallet under maintenance, should be updated soon )



Characteristics
---------------------------------------------------------------------------


- Hash Algorithm : Yespower POW algorithm    ( CPU only )

( With the addition of modified Dash's DarkGravityWave v3 algorithm, that changes difficulty every block, and progressively lowers difficulty if long stale tips happens. Tested and fully working !!)

- The Hive : A " hodl and profit " mining system accessible to everyone, that works side by side with POW mining, invented by  LitecoinCash's developers !!

- Total Supply : 210,000,000 LNC

- Difficulty adjusted every block and possibly in CURRENT block if a long network stale tip happens !! This protects LightningCash from high hash variations and " high hash attacks " , that happens frequently for new coins and kills them by making the mining difficulty too high for the " normal " network's hash rate. 

- Changed famous NOMP ( node open mining portal ) pool software to include possible CURRENT block difficulty change and send new work to miners if so.

- 5 seconds block time. ( Actual average block time is 5.1 seconds. )

- Number of confirmations needed for a transaction : 6  (transaction is confirmed in approximately 30.6 seconds)

- Premine : 2,200,000 coins, barely above 1 % of max supply

- Block Reward : 5 LightningCash - halved every 8 400 000 blocks ( currently at 2.5 LNC / block ) 

- Synchronising the wallet takes about 1 to 2 hours with a normal download speed.
  A bootstrap is available here : https://easyupload.io/lgva9x ( made on August 10th 2022 )



LightningCash is based on LitecoinCash Core, Litecoin Core and Bitcoin Core's open source codes.
( Bitcoin ---> Litecoin ---> LitecoinCash ---> LightningCash )
LightningCash is an independant coin and is not associated in any way with LitecoinCash's current team !


License
-------

Lightning-Cash Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.


Development Process
-------------------

You can fork LightningCash github's repository, tweak the code and suggest pull request if you have bug fixes or improvements to propose.

Testing
-------

For now, testing is done privately before releasing new wallet's versions.

Translations
------------

Any translation corrections or expansions are welcomed as GitHub pull requests.
