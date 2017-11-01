//Fischer Thomas gs16m022

function togglePause()
{
  if(debug)
    console.log("PAUSE!");
  gamePause = !gamePause;
}

//some init stuff
var gamePause = true,
    canvas,
    context,
    gameLoopInterval = undefined,
    startFrame = undefined,
    lastFrame = 0,
    //deltaTime = 0,
    scorefield,
    highscore = 0,
    highscoreField,
    gotInit = false,
    debug = true,
    blockSize,
    blocks = [];

function startGame()
{
  if(debug)
    $("#debug").show();
  else {
    $("#debug").hide();
  }

  viewport.off("click");
  viewport.click("click", click);

  togglePause();

  //gameLoopInterval = setInterval(gameLoopFixed, 1000);
  //window.requestAnimationFrame(gameLoop);
}

function click()
{
  if(!gamePause)
  {
    if(debug)
      console.log("Jump!");
  }
  else {
    if(debug)
      console.log("Put Your Hands Up In The Air!");
  }
}

//handler
$(document).ready(function() {
  init();
});

function init(){
  //Original Tetris: 10 blocks wide, 18 blocks high
  canvas = document.getElementById('viewport');
  canvas.height = window.innerHeight;
  canvas.width = 10 * canvas.height / 18;
  context = canvas.getContext('2d');

  blockSize = canvas.width / 10;
  blocks[0] = {};
  blocks[0].x = 0;
  blocks[0].y = 0;

  if(gotInit == false)
  {
    $(document).keydown(function(event)
    {
      if(debug)
        console.log(event);
      switch(event.key)
      {
        case "Escape":
        if(gamePause == false)
          togglePause();
        break;
      }
    });
  }

  gotInit = true;

  highscore = 0;
  togglePause();

  gameLoopInterval = setInterval(function(){gameLoop(500);}, 500);
  //window.requestAnimationFrame(requestFrameHandler);
}

function requestFrameHandler(timeAlive)
{
  if(startFrame == undefined)
  {
    startFrame = timeAlive;
  }

  timeAlive -= startFrame;
  deltaTime = (timeAlive - lastFrame)*0.001;//ms to s conversion
  lastFrame = timeAlive;

  if(deltaTime > 0.1) //safety precaution
    deltaTime = 0.1;

  gameLoop(deltaTime);

  console.log(deltaTime);

  window.requestAnimationFrame(requestFrameHandler);
}

function gameLoop(deltaTime)
{
  if(gamePause)
    return;

  context.save();

  context.fillStyle = 'lightgrey';
  context.fillRect(0, 0, 480, 720);

  for(var i = 0; i < blocks.length; i++)
  {
    context.save();
    context.fillStyle = 'rgb(200, 0, 0)';
    context.fillRect(blocks[i].x * blockSize, blocks[i].y * blockSize, blockSize, blockSize);
    context.restore();
  }
  context.restore();

  for(var i = 0; i < blocks.length; i++)
  {
    if(blocks[i].y + 1 < 18)
      blocks[i].y++;
  }

  //$("#debug").html(deltaTime);
}
