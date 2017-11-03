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
  tmp.x = 4;
  tmp.y = 0;
  tmp.angle = 0;
  tmp.style = type;
  tmp.mesh = blockStyles[type];

  return tmp;
}

//some init stuff
var gamePause = true,
    context,
    collisionContext,
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
    activeBlock,
    blockCount = 0,
    colorBg = 'rgb(196, 207, 161)',
    colorBlock = 'rgb(107, 115, 83)';

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

function moveActiveBlock(mode)
{
  switch(mode)
  {
    case "left":
      if(activeBlock.x > 0)
        activeBlock.x--;
    break;

    case "right":
      if(activeBlock.x + 1 < 10)
        activeBlock.x++;
    break;

    case "down":
      console.log("moving active block down is not implemented yet!");
    break;
  }
}

function rotateActiveBlock()
{
  if(activeBlock.style != 5)
    activeBlock.angle += 90;
  if(activeBlock.angle >= 360)
    activeBlock.angle = 0;
}

function countBlocks()
{
  var count = 0,
      data = collisionCtx.getImageData(0, 0, 10, 20);

  for(var x = 0; x < 10; x++)
  {
    for(var y = 0; y < 20; y++)
    {
      var pixel = data.data[((y * (10 * 4)) + (x * 4))];
    }
  }

  return count;
}

//handler
$(document).ready(function() {
  init();
});

function init(){
  //Original Tetris: 10 blocks wide, 18 blocks high
  var canvas = document.getElementById('viewport');
  canvas.height = window.innerHeight;
  canvas.width = 10 * canvas.height / 20;
  context = canvas.getContext('2d');
  context.imageSmoothingEnabled = false;

  //https://stackoverflow.com/questions/43369748/how-to-render-offscreen-canvas-properly
  var collisionCanvas = document.createElement('canvas');
  collisionCanvas.height = 20;
  collisionCanvas.width = 10;
  collisionCtx = collisionCanvas.getContext('2d');

  blockSize = canvas.width / 10;

  blockStyles = [];
  //Line
  blockStyles[0] = new Path2D();
  for(var i = 0; i < 4; i++)
  {
    blockStyles[0].rect(0, -(i-1) * 1, 1, 1);
  }
  //L
  blockStyles[1] = new Path2D();
  blockStyles[1].rect(0, 1,  1, 1);
  blockStyles[1].rect(0, 0, 1, 1);
  blockStyles[1].rect(0, -1, 1, 1);
  blockStyles[1].rect(1, 1,  1, 1);
  //L2
  blockStyles[2] = new Path2D();
  blockStyles[2].rect(0,1,  1, 1);
  blockStyles[2].rect(0,0,1, 1);
  blockStyles[2].rect(0,-1, 1, 1);
  blockStyles[2].rect(-1, 1, 1, 1);
  //var1
  blockStyles[3] = new Path2D();
  blockStyles[3].rect(0,1,  1, 1);
  blockStyles[3].rect(0,0,1, 1);
  blockStyles[3].rect(-1,0,1, 1);
  blockStyles[3].rect(1, 1,  1, 1);
  //var2
  blockStyles[4] = new Path2D();
  blockStyles[4].rect(0,1,  1, 1);
  blockStyles[4].rect(0,0,1, 1);
  blockStyles[4].rect(-1,1,  1, 1);
  blockStyles[4].rect(1, 0,1, 1);
  //cube
  blockStyles[5] = new Path2D();
  blockStyles[5].rect(0,1,  1, 1);
  blockStyles[5].rect(0,0,1, 1);
  blockStyles[5].rect(1, 0,1, 1);
  blockStyles[5].rect(1, 1,  1, 1);
  //var3
  blockStyles[6] = new Path2D();
  blockStyles[6].rect(0,-1,  1, 1);
  blockStyles[6].rect(0,0,1, 1);
  blockStyles[6].rect(-1,0,1, 1);
  blockStyles[6].rect(1, 0,  1, 1);

  blocks[0] = createTetrisBlock(6);
  activeBlock = blocks[0];

  if(gotInit == false)
  {
    $(document).keydown(function(event)
    {
      if(debug && false)
        console.log(event);
      switch(event.key)
      {
        case "A"://fallthrough
        case "a":
          moveActiveBlock("left");
        break;

        case "D"://fallthrough
        case "d":
          moveActiveBlock("right");
        break;

        case "W"://fallthrough
        case "w":
          rotateActiveBlock();
        break;

        case "S"://fallthrough
        case "s":
          moveActiveBlock("down");
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
      if(blocks[i].y + 1 < 20)
        blocks[i].y++;
    }

    this.counter = 0;
  }

  collisionCtx.save();
  collisionCtx.fillStyle = colorBg;
  collisionCtx.fillRect(0, 0, 10, 20);

  for(var i = 0; i < blocks.length; i++)
  {
    collisionCtx.save();
    collisionCtx.fillStyle = colorBlock;
    collisionCtx.translate(blocks[i].x * 1 + 0.5, blocks[i].y + 0.5);
    collisionCtx.rotate(blocks[i].angle * 0.01745329251); //Math.PI / 180
    collisionCtx.translate(-0.5, -0.5);
    collisionCtx.fill(blocks[i].mesh);
    collisionCtx.restore();
  }
  collisionCtx.restore();

  context.save();
  context.scale(blockSize, blockSize);
  context.drawImage(collisionCtx.canvas, 0, 0);
  context.restore();

  //$("#debug").html(deltaTime);
}
