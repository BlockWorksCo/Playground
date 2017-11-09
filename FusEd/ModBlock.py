
"""
                  SEEK_SET                                    SEEK_END
                     |                                           |
File:                ---------------------------------------------
                         | |       ___|____     \______/
                        /   \     /        \        |
ModificationBlocks:     +++++     ++++++++++     
                       overlay    insertion      deletion
                          & 
                      insertion

Note: These are byte-ranges, not lines.

"""


import sys


class ModificationBlock:
    """
    A few cases exist:
    
    deletion:  end-start > 0 AND len(data) == 0
    insertion: end == start  AND len(data) > 0
    overlay:   end-start > 0 AND len(data) > 0
    unused     end == start  AND len(data) == 0

    """

    start   = -1
    end     = -1
    data    = None

    def __init__(self, start,end, data):

        self.start  = start
        self.end    = end
        self.data   = data



class PassthruBlock:
    """
    """

    start   = -1
    end     = -1
    data    = None

    def __init__(self, start,end, data):

        self.start  = start
        self.end    = end
        self.data   = data





class ModificationBlockList:
    """
    1) Simple insert:
                -------------------------------
                    ++++++
        becomes:
                ----++++++---------------------

    2) Beginning insert:
                -------------------------------
                +++++
        becomes:
                +++++--------------------------

    3) End insert:
                -------------------------------
                                            +++
        becomes:
                ----------------------------+++

    4) straddling insert:
                1111111122222222222222222222222
                     ++++++
        becomes:
                11111++++++22222222222222222222

    5) deletion:
                1111111111111111111111111111111
                      ------------ 
        becomes:
                1111112222222222222
    """

    modList = []

    def Add(self, newBlock):

        startBlock  = self.FindBlockContainingPoint(newblock.start)
        endBlock    = self.FindBlockContainingPoint(newblock.end)

        if startBlock == endBlock:
            startBlock.end = newBlock.start
            self.modList.append(newBlock)
            self.modList.append(Modification)
        else:
            print('<Cant handle this case yet>')


    def FindBlockContainingPoint(self, position):
        """
        """
        for block in self.modList:
            if position >= block.start and position < block.end:
                return block

        return None


    def Show(self):
        """
        """
        for block in self.modList:
            sys.stdout.write(block.data)


if __name__ == '__main__':
    
    blockList   = ModificationBlockList()

    blockList.Add(PassthruBlock(0,50,'0123456789'*5))
    blockList.Add(ModificationBlock(10,10,'abc'))

    blockList.Show()





