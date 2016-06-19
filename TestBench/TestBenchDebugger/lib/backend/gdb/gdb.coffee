{BufferedProcess, Emitter} = require 'atom'

module.exports =
  class GDB

    STATUS =
      NOTHING: 0
      RUNNING: 1
      STOPPED: 2
      DESTROYED: 3

    constructor: (executable, target) ->
      @token = 0
      @handler = {}
      @jsonText = ''
      @emitter = new Emitter

      stdout = (lines) =>
        for line in lines.split('\n')
            @jsonText    = @jsonText + line
            if line == '{'
                @jsonText   = line
            if line == '}'
                #console.log(@jsonText)
                response    = JSON.parse(@jsonText)
                @processReponse(response)
                @jsonText    = ''

      stderr = (lines) =>
        console.info(lines)

      args = [target] #
      command = executable
      console.log(executable)
      console.log(target)
      @process = new BufferedProcess({command, args, stdout, stderr}).process
      @stdin = @process.stdin
      @status = STATUS.NOTHING

    destroy: ->
      @status = STATUS.DESTROYED
      @process.kill()
      @emitter.dispose()

    processReponse: (response) ->
      switch response.type
          when 'PositionReport' then @emitter.emit 'position-report',response
          when 'StateChange' then console.log('<StateChange!>')
      console.log(response)


    isRunning: ->
      @status is STATUS.RUNNING

    isStopped: ->
      @status is STATUS.STOPPED

    isDestroyed: ->
      @status is STATUS.DESTROYED

    onPositionReport: (callback) ->
      @emitter.on 'position-report', callback

    onExecAsyncOutput: (callback) ->
      @emitter.on 'exec-async-output', callback

    onExecAsyncStopped: (callback) ->
      @status = STATUS.STOPPED
      @emitter.on 'exec-async-stopped', callback

    onExecAsyncRunning: (callback) ->
      @status = STATUS.RUNNING
      @emitter.on 'exec-async-running', callback

    listFiles: (handler) ->
      @postCommand 'file-list-exec-source-files', (clazz, result) =>
        files = []
        if clazz == RESULT.DONE
          for file in result.files
            files.push(file.fullname)
        handler(files)

    listExecFile: (handler) ->
      @postCommand 'file-list-exec-source-file', (clazz, result) =>
        file = null
        if clazz == RESULT.DONE
          file = result
        handler(file)

    setSourceDirectories: (directories, handler) ->
      args = []
      args.push("\"#{directory}\"") for directory in directories

      command = 'environment-directory ' + args.join(' ')
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE)

    listBreaks: (handler) ->
      @postCommand 'break-list', (clazz, result) =>
        breaks = []
        if clazz == RESULT.DONE and result.BreakpointTable.body.bkpt
          breaks = result.BreakpointTable.body.bkpt
        handler(breaks)

    deleteBreak: (number, handler) ->
      command = "del #{number}"
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE)

    disassembleData: ({address, file, mode}, handler) ->
      args = []
      if address
        args.push("-s #{address.start}")
        args.push("-e #{address.end}")
      else if file
        args.push("-f #{file.name}")
        args.push("-l #{file.linenum}")
        args.push("-n #{file.lines}") if file.lines
      args.push("-- #{mode}")

      command = 'data-disassemble ' + args.join(' ')

      @postCommand command, (clazz, result) =>
        instructions = []
        if clazz == RESULT.DONE
          instructions = result.asm_insns.src_and_asm_line
        handler(instructions)

    insertBreak: ({location, condition, count, thread, temporary, hardware, disabled, tracepoint}, handler) ->

      command = 'b #{location}'

      @postCommand command, (clazz, result) =>
        abreak = null
        if clazz == RESULT.DONE
          abreak = result.bkpt
        handler(abreak)

    run: (handler) ->
      command = 'go'
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.RUNNING)

    continue: (handler) ->
      command = 'go'
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.RUNNING)

    interrupt: (params, handler) ->
      command = 'break'
      handler = params if not handler
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE)

    next: (handler) ->
      command = 'next'
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.RUNNING)

    step: (handler) ->
      command = 'step'
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.RUNNING)

    finish: (handler) ->
      command = 'quit'
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.RUNNING)

    exit: (handler) ->
      command = 'quit'
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE)

    go: (handler) ->
      command = 'go'
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE)

    set: (key, value, handler) ->
      command = "set #{key} #{value}"
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE)

    target: (command, params, handler = (c,r) ->) ->
      if handler
        command = "target-#{command} #{params}"
      else
        command = "target-#{command}"
        handler = params
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE, result)

    stack: (command, params, handler = (c,r) ->) ->
      if handler
        command = "stack-#{command} #{params}"
      else
        command = "stack-#{command}"
        handler = params if params
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE, result)

    var: (command, params, handler = (c,r) ->) ->
      if handler
        command = "var-#{command} #{params}"
      else
        command = "var-#{command}"
        handler = params if params
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE, result)

    data: (command, params, handler = (c,r) ->) ->
      if handler
        command = "data-#{command} #{params}"
      else
        command = "data-#{command}"
        handler = params if params
      @postCommand command, (clazz, result) =>
        handler(clazz == RESULT.DONE, result)

    postCommand: (command, handler) ->
      @stdin.write("#{command}\n")
