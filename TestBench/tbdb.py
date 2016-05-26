



import bdb





class tbdb(bdb.Bdb):

    run = 0

    def user_call(self, frame, args):
        """
        """
        name = frame.f_code.co_name or "<unknown>"
        print("user_call", name, args)
        #self.set_continue() # continue


    def user_line(self, frame):
        """
        """
        if self.run:
            self.run = 0
            self.set_trace() # start tracing
        else:
            # arrived at breakpoint
            name = frame.f_code.co_name or "<unknown>"
            filename = self.canonic(frame.f_code.co_filename)
            print("user_line at", filename, frame.f_lineno, "in", name)
        #print("continue1...")
        #self.set_continue() # continue to next breakpoint


    def user_return(self, frame, value):
        """
        """
        name = frame.f_code.co_name or "<unknown>"
        print("user_return from", name, value)
        #self.set_continue() # continue


    def user_exception(self, frame, exception):
        """
        """
        name = frame.f_code.co_name or "<unknown>"
        print("exception in", name, exception)
        #self.set_continue() # continue


