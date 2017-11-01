//Fischer Thomas gs16m022

function togglePause()
{
  if(debug)
    console.log("PAUSE!");
  gamePause = !gamePause;
}

function createTetrisBlock(type)
{
  var tmp = {};
  tmp.x = 0;
  tmp.y = 0;
  tmp.angle = 0;
  tmp.style = type;
  tmp.mesh = blockStyles[type];

  return tmp;
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
    blockStyles,
    blocks = [],
    activeBlock;

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

  blockStyles = [];
  //Line
  blockStyles[0] = new Path2D();
  for(var i = 0; i < 4; i++)
  {
    blockStyles[0].rect(0, -i * blockSize, blockSize, blockSize);
  }
  blockStyles[0].origin = {};
  blockStyles[0].origin.x = blockSize * 0.5;
  blockStyles[0].origin.y = -blockSize * 0.5;
  //L
  blockStyles[1] = new Path2D();
  blockStyles[1].rect(0,         blockSize,  blockSize, blockSize);
  blockStyles[1].rect(0,         0,          blockSize, blockSize);
  blockStyles[1].rect(0,         -blockSize, blockSize, blockSize);
  blockStyles[1].rect(blockSize, blockSize,  blockSize, blockSize);
  blockStyles[1].origin = {};
  blockStyles[1].origin.x = blockSize * 0.5;
  blockStyles[1].origin.y = blockSize * 0.5;
  //L2
  blockStyles[2] = new Path2D();
  blockStyles[2].rect(0,         blockSize,  blockSize, blockSize);
  blockStyles[2].rect(0,         0,          blockSize, blockSize);
  blockStyles[2].rect(0,         -blockSize, blockSize, blockSize);
  blockStyles[2].rect(-blockSize, blockSize, blockSize, blockSize);
  blockStyles[2].origin = {};
  blockStyles[2].origin.x = blockSize * 0.5;
  blockStyles[2].origin.y = blockSize * 0.5;
  //var1
  blockStyles[3] = new Path2D();
  blockStyles[3].rect(0,         blockSize,  blockSize, blockSize);
  blockStyles[3].rect(0,         0,          blockSize, blockSize);
  blockStyles[3].rect(-blockSize,0,          blockSize, blockSize);
  blockStyles[3].rect(blockSize, blockSize,  blockSize, blockSize);
  blockStyles[3].origin = {};
  blockStyles[3].origin.x = blockSize * 0.5;
  blockStyles[3].origin.y = blockSize * 0.5;
  //var2
  blockStyles[4] = new Path2D();
  blockStyles[4].rect(0,         blockSize,  blockSize, blockSize);
  blockStyles[4].rect(0,         0,          blockSize, blockSize);
  blockStyles[4].rect(-blockSize,blockSize,  blockSize, blockSize);
  blockStyles[4].rect(blockSize, 0,          blockSize, blockSize);
  blockStyles[4].origin = {};
  blockStyles[4].origin.x = blockSize * 0.5;
  blockStyles[4].origin.y = blockSize * 0.5;
  //cube
  blockStyles[5] = new Path2D();
  blockStyles[5].rect(0,         blockSize,  blockSize, blockSize);
  blockStyles[5].rect(0,         0,          blockSize, blockSize);
  blockStyles[5].rect(blockSize, 0,          blockSize, blockSize);
  blockStyles[5].rect(blockSize, blockSize,  blockSize, blockSize);
  blockStyles[5].origin = {};
  blockStyles[5].origin.x = blockSize * 0.5;
  blockStyles[5].origin.y = blockSize * 0.5;
  //var3
  blockStyles[6] = new Path2D();
  blockStyles[6].rect(0,         -blockSize,  blockSize, blockSize);
  blockStyles[6].rect(0,         0,          blockSize, blockSize);
  blockStyles[6].rect(-blockSize,0,          blockSize, blockSize);
  blockStyles[6].rect(blockSize, 0,  blockSize, blockSize);
  blockStyles[6].origin = {};
  blockStyles[6].origin.x = blockSize * 0.5;
  blockStyles[6].origin.y = blockSize * 0.5;

  blocks[0] = createTetrisBlock(6);
  activeBlock = blocks[0];

  if(gotInit == false)
  {
    $(document).keydown(function(event)
    {
      if(debug)
        console.log(event);
      switch(event.key)
      {
        case "A"://fallthrough
        case "a":
          if(activeBlock.x > 0)
            activeBlock.x--;
        break;

        case "D"://fallthrough
        case "d":
          if(activeBlock.x + 1 < 10)
            activeBlock.x++;
        break;

        case "W"://fallthrough
        case "w":
          if(activeBlock.style != 5)
            activeBlock.angle += 90;
          if(activeBlock.angle >= 360)
            activeBlock.angle = 0;
        break;

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

  //gameLoopInterval = setInterval(function(){gameLoop(500);}, 500);
  window.requestAnimationFrame(requestFrameHandler);
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

  //console.log(deltaTime);

  window.requestAnimationFrame(requestFrameHandler);
}

function gameLoop(deltaTime)
{
  if(gamePause)
    return;

  if(this.counter == undefined)
    this.counter = 0;

  this.counter += deltaTime;

  if(this.counter >= 0.5)
  {
    for(var i = 0; i < blocks.length; i++)
    {
      if(blocks[i].y + 1 < 18)
        blocks[i].y++;
    }

    this.counter = 0;
  }

  context.save();

  context.fillStyle = '#c4cfa1';
  context.fillRect(0, 0, 480, 720);

  for(var i = 0; i < blocks.length; i++)
  {
    context.save();
    context.fillStyle = '#6b7353';
    context.translate(blocks[i].x * blockSize + (blockSize * 0.5), blocks[i].y * blockSize);
    context.rotate(blocks[i].angle * 0.01745329251); //Math.PI / 180
    context.translate(-blocks[i].mesh.origin.x, -blocks[i].mesh.origin.y);
    context.fill(blocks[i].mesh);
    context.restore();
  }
  context.restore();

  //$("#debug").html(deltaTime);
}
