#!/usr/bin/env python3
import pstats
import sys
p = pstats.Stats(sys.argv[1])
p.sort_stats('cumulative').print_stats(100)

