<img src="https://i.ibb.co/3rvpw0v/68747470733a2f2f692e6962622e636f2f52767971784b4e2f626974636f696e2e706e67.png" alt="68747470733a2f2f692e6962622e636f2f52767971784b4e2f626974636f696e2e706e67" border="0"></a><br /><a target='_blank' href='https://imgbb.com/'></a><br />

# LightningCash Core integration/staging tree
===========================================
LightningCash Core new release version : 9.2.0

README.md Updated on September 7th 2022


Bitcointalk ANN : https://bitcointalk.org/index.php?topic=5117428


What is LightningCash ?
----------------------

LightningCash is a coin based on LitecoinCash ( LCC ), with Yespower POW algorithm, including DarkGravityWave ( difficulty changes for every block ) and possible CURRENT block difficulty change over time to protect against high network hash variation and long stale tip. It is also 15 times faster than LitecoinCash. For full details, please visit our website at https://lightningcash-coin.com . Compiled binary releases are available here on Github.
Whitepapers will be available soon.

Current supported version is 9.0.0 , 9.1.0 or 9.2.0

Lightning-Cash Core is the full node software that makes up the backbone of the LNC network.


Website : https://lightningcash-coin.com

Pool : https://pool.lightningcash-coin.com

Block Explorer : https://explorer.lightningcash-coin.com

Discord Server : https://discord.gg/WrMVXcA8bt


Exchanges : 

Very probable : 
https://atomicdex.io/  ( scraping through the code... )

Too expensive for now :
https://xt.com/
https://binance.com/
https://pro.coinbase.com/
https://www.kucoin.com/

Not sure... would be cool if not too expensive :
https://www.stex.com/
https://mercatox.com/ 
https://main.southxchange.com/
https://hitbtc.com/
https://instaswap.io/
https://exrates.me/


Characteristics
---------------------------------------------------------------------------


- Hash Algorithm : Yespower POW algorithm    ( CPU only )

( With the addition of modified Dash's DarkGravityWave v3 algorithm, that changes difficulty every block, and progressively lowers difficulty if long stale tips happens. Tested and fully working !!) ---> check DarkGravity V3 (https://github.com/dashpay/dash/blob/master/src/pow.cpp#L82) (By Evan Duffield)
 <evan@dash.org>

And check my modifications here :        https://github.com/MerlinMagic2018/Lightning-Cash/blob/main/src/pow.cpp#L110

( Difficulty adjusted every block and possibly in CURRENT block if a long network stale tip happens !! This protects LightningCash from high hash variations and " high hash attacks " , that happens frequently for new coins and kills them by making the mining difficulty too high for the " normal " network's hash rate. )


- The Hive : A " hodl and profit " mining system accessible to everyone, that works side by side with POW mining, invented by  LitecoinCash's developers !!

- Maximum Supply : 113,996,811 LNC 

( 2200000-->(Premine, first 4 blocs) + 3096-->( slow start mining block 5 to 124) + (617652 x 50)-->(bloc 125 to 617776)
+ ((8399999 - 617776) * 5 )  +  ( 8400000 * 2.5 ) + (8400000 * 1.25)  +  ( 8400000 * 0.625 ) etc..... = 113996811 LNC

- Current Supply : 89,840,581 LNC ( @ Block # 15 537 507 , September 7th 2022 ) ---> 78.81 % of max supply

- Next halving : Block #16800000 , November 19th 2022 ---> 81.58 % of max supply

- Third halving : Block #25200000 , March 18th 2024 ---> 90.79 % of max supply

- Changed famous NOMP ( node open mining portal ) pool software to include possible CURRENT block difficulty change and send new work to miners if so.
Reference : https://bitcointalk.org/index.php?topic=5119004   and  https://bitcointalk.org/index.php?topic=5119550

- 5 seconds block time. ( In chainparams.cpp. is set at 10 seconds, with hive, makes it 5s . Actual average block time is 5.1 seconds. )

- Number of confirmations needed for a transaction : 6  (transaction is confirmed in approximately 30.6 seconds)

- Premine : 2,200,000 coins, just 1.93 % of max supply

- Block Reward : 5 LightningCash - halved every 8 400 000 blocks ( currently at 2.5 LNC / block ) 

- Synchronising the wallet takes about 8 hours with a normal download speed.
  A bootstrap is available here : https://easyupload.io/lgva9x ( made on August 10th 2022 )



LightningCash is based on LitecoinCash Core, Litecoin Core and Bitcoin Core's open source codes.
( Bitcoin ---> Litecoin ---> LitecoinCash ---> LightningCash )
LightningCash is an independant coin and is not associated with LitecoinCash's team
( but if Tanner want to help he is always welcomed )


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
