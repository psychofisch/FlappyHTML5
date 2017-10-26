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
    deltaTime = 0,
    scorefield,
    highscore = 0,
    highscoreField,
    gotInit = false,
    debug = true,
    block;

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

  window.requestAnimationFrame(gameLoop);
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
  canvas = document.getElementById('viewport');
  context = canvas.getContext('2d');

  block = new Path2D();
  block.rect(10, 10, 50, 50);

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
  window.requestAnimationFrame(gameLoop);
}

function gameLoop(timeAlive)
{
  if(gamePause)
    return;

  if(startFrame == undefined)
  {
    startFrame = timeAlive;
  }

  timeAlive -= startFrame;
  deltaTime = (timeAlive - lastFrame)*0.001;//ms to s conversion
  lastFrame = timeAlive;

  if(deltaTime > 0.1) //safety precaution
    deltaTime = 0.1;

  highscore += 100 * deltaTime;
  if(highscore > 480)
    highscore = -50;

  context.save();

  context.clearRect(0, 0, 480, 720);
  context.translate(highscore, 0);

  context.fillStyle = 'rgb(200, 0, 0)';
  context.fill(block);

  context.restore();

  //$("#debug").html(deltaTime);

  window.requestAnimationFrame(gameLoop);
}
