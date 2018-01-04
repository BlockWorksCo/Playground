// The application will create a renderer using WebGL, if possible,
// with a fallback to a canvas render. It will also setup the ticker
// and the root stage PIXI.Container
const app = new PIXI.Application({ transparent: false, backgroundColor : 0xbbbbbb });
app.backgroundColor = 0x061639;

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


let bunny;


function HandleUpKeyPress()
{
    bunny.vy   = -1;
    console.log('Up press');
}

function HandleUpKeyRelease(keyCode)
{
    bunny.vy   = 0;
    console.log('Up release');
}

function HandleDownKeyPress()
{
    bunny.vy   = 1;
    console.log('Down press');
}

function HandleDownKeyRelease(keyCode)
{
    bunny.vy   = 0;
    console.log('Down release');
}

function HandleLeftKeyPress()
{
    bunny.vx   = -1;
    console.log('Left press');
}

function HandleLeftKeyRelease(keyCode)
{
    bunny.vx   = 0;
    console.log('Left release');
}

function HandleRightKeyPress()
{
    bunny.vx   = 1;
    console.log('Right press');
}

function HandleRightKeyRelease(keyCode)
{
    bunny.vx   = 0;
    console.log('Right release');
}

function keyboard(keyCode, press, release) 
{
  let key = {};
  key.code = keyCode;
  key.isDown = false;
  key.isUp = true;
  key.press = press;
  key.release = release;

  //The `downHandler`
  key.downHandler = event => {
    if (event.keyCode === key.code) {
      if (key.isUp && key.press) key.press();
      key.isDown = true;
      key.isUp = false;
      key.press();
    }
    event.preventDefault();
  };

  //The `upHandler`
  key.upHandler = event => {
    if (event.keyCode === key.code) {
      if (key.isDown && key.release) key.release();
      key.isDown = false;
      key.isUp = true;
      key.release();
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


function SetupLevel()
{
    levelData   = 
    [
        {"Name":"BlockOne.png", "X":10,"Y":10, "SX":0.2,"SY":0.2},    
        {"Name":"BlockTwo.png", "X":120,"Y":10, "SX":0.2,"SY":0.2},    
        {"Name":"BlockOne.png", "X":220,"Y":10, "SX":0.2,"SY":0.2},    
        {"Name":"BlockThree.png", "X":320,"Y":10, "SX":0.2,"SY":0.2},    
    ];

    for(i=0; i<levelData.length; i++)
    {
        sprite = new PIXI.Sprite.fromImage(levelData[i]["Name"]);
        sprite.x        = levelData[i]["X"];
        sprite.y        = levelData[i]["Y"];
        sprite.scale.x  = levelData[i]["SX"];
        sprite.scale.y  = levelData[i]["SY"];
        app.stage.addChild(sprite);
    }
}


//
//
//
function Iterate(app)
{
    bunny.rotation  += bunny.vr;
    bunny.x         += bunny.vx;
    bunny.y         += bunny.vy;
}

//
//
//
let keyObjectUp     = keyboard(38, HandleUpKeyPress, HandleUpKeyRelease);
let keyObjectDown   = keyboard(40, HandleDownKeyPress, HandleDownKeyRelease);
let keyObjectLeft   = keyboard(37, HandleLeftKeyPress, HandleLeftKeyRelease);
let keyObjectRight  = keyboard(39, HandleRightKeyPress, HandleRightKeyRelease);

stickMan = new PIXI.Sprite.fromImage("StickMan.png");
stickMan.x  = 10;
stickMan.y  = 10;
stickMan.scale.x    = 0.2;
stickMan.scale.y    = 0.2;
app.stage.addChild(stickMan);

SetupLevel();

bunny = new PIXI.Sprite.fromImage("Bunny.png");
// Setup the position of the bunny
bunny.x = app.renderer.width / 2;
bunny.y = app.renderer.height / 2;

// Rotate around the center
bunny.anchor.x = 0.5;
bunny.anchor.y = 0.5;

bunny.vx   = 0;
bunny.vy   = 0;
bunny.vr   = 0.01;

// Add the bunny to the scene we are building
app.stage.addChild(bunny);

// Listen for frame updates
app.ticker.add(() => {
     // each frame we spin the bunny around a bit
    Iterate(this)
});



