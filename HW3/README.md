# Testy Cache
## Ariel Flaster and Sarah Strand 

### Our Tests

The tests in our test files have pretty good code coverage. For each test we call a makeCache function, which, unless other arguments are included, makes a cache of size 2048 and populates it with a single key/value pair, <“apple”, “four”>. This cache object uses our fifo_evictor. We then call several other functions to ensure functionality.

| Test Name | Description | Status |
| --------- | ----------- | ------ |
| testGet | get method is tested | pass |
| testGetNull | get method is tested, where the item we're trying to get is not actually in the cache | pass | 
| testDel | del method is tested | pass |
| testDelNull | del method is tested, where the item we're trying to del is not actually in the cache | pass | 
| testSpaceUsed | tests spaceUsed method | pass |
| testReset | tests reset method | pass |
| testSameKey | tests what happens when we add something with the same key twice, check to make sure it updates as expected | pass |
| testEvictorWithFullCache | tests evictor functionality when the cache is full and we try to add a new item | pass |
| testEvictorEvictingSameItemTwice | test evictor when we have added the same item twice, makes sure that the evictor doesn't try to evict the same key twice | pass | 


### Testing Jon & Eli

| Test Name | Status |
| --------- | ------ |
| testGet | pass |
| testGetNull | pass | 
| testDel | pass |
| testDelNull | pass | 
| testSpaceUsed | pass |
| testReset | pass |
| testSameKey | pass |
| testEvictorWithFullCache | pass |
| testEvictorEvictingSameItemTwice | pass | 

All tests passed. Great minds think alike!


### Testing Maxx & Casey

| Test Name | Status |
| --------- | ------ |
| testGet | fail |
| testGetNull | pass | 
| testDel | pass |
| testDelNull | pass | 
| testSpaceUsed | pass |
| testReset | pass |
| testSameKey | fail |
| testEvictorWithFullCache | pass |
| testEvictorEvictingSameItemTwice | pass | 

Their code is very hard to read, so it was hard to find what went wrong here. 


### Testing Mason & Sebastion 

| Test Name | Status |
| --------- | ------ |
| testGet | ? |
| testGetNull | ? | 
| testDel | ? |
| testDelNull | ? | 
| testSpaceUsed | ? |
| testReset | ? |
| testSameKey | ? |
| testEvictorWithFullCache | ? |
| testEvictorEvictingSameItemTwice | ? | 


