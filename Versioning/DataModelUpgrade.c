//
// This code will have to be generated manually as it will potentially involve application-level logic, not simply
// straight field access.
//
// We can potentially do all of the following transforms and more:
// - Field rearrangements.
// - Field type changes.
// - New field additions.
// - Old field removals.
// - Unit changes.
// - Defaults for new fields.
// - Values being recalculated.
//


#include "DataModelUpgrade.h"
#include "DataModelMap.h"
#include "BlockDevice.h"




//
//
//
static DataModelMap     map;
static DataModelMap_v0  map_v0;



//
// Upgrade from v0 to v1.
// Potentially here you could have a table of versionNumber->UpgradeFn in order to support multiple
// old versions.
//
void DataModelUpgrade()
{
    //
    // Read the old....
    //
    BlockDeviceRead(0, sizeof(DataModelMap_v0), (uint8_t*)&map_v0 );

    //
    // Perform the Upgrade. We can do any transform here, not just copying.
    //
    map.one     = map_v0.one;
    map.two     = map_v0.two;
    map.three   = map_v0.three;
    map.four    = map_v0.four;

    map.version = 0x01;

    //
    // ...write the new.
    //
    BlockDeviceWrite(0, sizeof(DataModelMap), (uint8_t*)&map );
}

