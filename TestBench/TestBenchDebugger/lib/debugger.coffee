DebuggerView = require './debugger-view'
{CompositeDisposable} = require 'atom'
fs = require 'fs'

module.exports = Debugger =
  subscriptions: null

  config:
    gdbPath:
      type: 'string'
      default: "../PythonMain"
    adbPath:
      type: 'string'
      default: "Path to the adb"
    libSearchPath:
      type: 'string'
      default: "Shared library search path"
    targetBinary:
      type: 'string'
      default: 'Target Binary'
    processId:
      type: 'string'
      default: ''

  activate: (state) ->
    # Events subscribed to in atom's system can be easily cleaned up with a CompositeDisposable
    @subscriptions = new CompositeDisposable

    # Register command that toggles this view
    @subscriptions.add atom.commands.add 'atom-workspace', 'testbench:toggle': => @toggle()
    @subscriptions.add atom.commands.add 'atom-workspace', 'core:close': =>
      @debuggerView?.destroy()
      @debuggerView = null
    @subscriptions.add atom.commands.add 'atom-workspace', 'core:cancel': =>
      @debuggerView?.destroy()
      @debuggerView = null

  deactivate: ->
    @subscriptions.dispose()
    # @openDialogView.destroy()
    @debuggerView?.destroy()

  serialize: ->

  toggle: ->
    if @debuggerView and @debuggerView.hasParent()
      @debuggerView.destroy()
      @debuggerView = null
    else
      # @openDialogView = new OpenDialogView (pid) =>
      target = atom.config.get("testbench-debugger.targetBinary")
      @debuggerView = new DebuggerView(target)