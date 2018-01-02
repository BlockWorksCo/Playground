// The application will create a renderer using WebGL, if possible,
// with a fallback to a canvas render. It will also setup the ticker
// and the root stage PIXI.Container
const app = new PIXI.Application();

// The application will create a canvas element for you that you
// can then insert into the DOM
document.body.appendChild(app.view);

//
// Make it fill the window.
//
app.renderer.view.style.position = "absolute";
app.renderer.view.style.display = "block";
app.renderer.autoResize = true;
app.renderer.resize(window.innerWidth, window.innerHeight);



function HandleKeyDown(keyCode)
{
    console.log('down: '+keyCode);
}

function HandleKeyUp(keyCode)
{
    console.log('Up: '+keyCode);
}

function keyboard(keyCode) 
{
  let key = {};
  key.code = keyCode;
  key.isDown = false;
  key.isUp = true;
  key.press = undefined;
  key.release = undefined;

  //The `downHandler`
  key.downHandler = event => {
    if (event.keyCode === key.code) {
      if (key.isUp && key.press) key.press();
      key.isDown = true;
      key.isUp = false;
        HandleKeyDown(key.code);
    }
    event.preventDefault();
  };

  //The `upHandler`
  key.upHandler = event => {
    if (event.keyCode === key.code) {
      if (key.isDown && key.release) key.release();
      key.isDown = false;
      key.isUp = true;
    HandleKeyUp(key.code);
    }
    event.preventDefault();
  };

  //Attach event listeners
  window.addEventListener(
    "keydown", key.downHandler.bind(key), false
  );
  window.addEventListener(
    "keyup", key.upHandler.bind(key), false
  );
  return key;
}




//
//
//
function Iterate(app)
{
    app.bunny.rotation += 0.01;
    app.bunny.x         += app.bunny.vx
    app.bunny.y         += app.bunny.vy
}

//
//
//
let keyObjectUp     = keyboard(38);
let keyObjectDown   = keyboard(40);
let keyObjectLeft   = keyboard(37);
let keyObjectRight  = keyboard(39);

// load the texture we need
PIXI.loader.add('bunny', 'Bunny.jpg').load((loader, resources) => {
    // This creates a texture from a 'bunny.png' image
    this.bunny = new PIXI.Sprite(resources.bunny.texture);

    // Setup the position of the bunny
    this.bunny.x = app.renderer.width / 2;
    this.bunny.y = app.renderer.height / 2;

    // Rotate around the center
    this.bunny.anchor.x = 0.5;
    this.bunny.anchor.y = 0.5;

    this.bunny.vx   = 0
    this.bunny.vy   = 0

    // Add the bunny to the scene we are building
    app.stage.addChild(this.bunny);

    // Listen for frame updates
    app.ticker.add(() => {
         // each frame we spin the bunny around a bit
        Iterate(this)
    });
});



