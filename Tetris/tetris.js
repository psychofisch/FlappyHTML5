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
  tmp.y = 4;
  tmp.angle = 0;
  tmp.style = type;
  tmp.mesh = blockStyles[type];
  blockCount += 4;

  return tmp;
}

//some init stuff
var gamePause = true,
    context,
    collisionContext,
    freezeContext,
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
      {
        activeBlock.x--;
        if(drawCtx(collisionCtx))
        {
          activeBlock.x++;
          drawCtx(collisionCtx);
        }
      }
    break;

    case "right":
      if(activeBlock.x + 1 < 10)
      {
        activeBlock.x++;
        if(drawCtx(collisionCtx))
        {
          activeBlock.x--;
          drawCtx(collisionCtx);
        }
      }
    break;

    case "down":
      console.log("moving active block down is not implemented yet!");
    break;
  }

  drawCtx(collisionCtx);
}

function rotateActiveBlock()
{
  if(activeBlock.style != 5)
    activeBlock.angle += 90;
  if(activeBlock.angle >= 360)
    activeBlock.angle = 0;

  if(drawCtx(collisionCtx))
  {
    if(activeBlock.angle == 0)
      activeBlock.angle = 360;
    activeBlock.angle -= 90;
    drawCtx(collisionCtx);
  }
}

function doesCollide()
{
  var count = 0,
      data = collisionCtx.getImageData(0, 0, 10, 20);

  for(var x = 0; x < 10; x++)
  {
    for(var y = 0; y < 20; y++)
    {
      var pixel = data.data[((y * (10 * 4)) + (x * 4))];
      if(pixel == colorBlock.substr(4,3))//compare red value
        count++;
    }
  }

  return (count != blockCount);
}

function isARowFull()
{
  var count = 0,
      data = collisionCtx.getImageData(0, 0, 10, 20);

  for(var y = 0; y < 20; y++)
  {
    count = 0;
    for(var x = 0; x < 10; x++)
    {
      var pixel = data.data[((y * (10 * 4)) + (x * 4))];
      if(pixel == colorBlock.substr(4,3))//compare red value
        count++;
    }

    if(count == 10)
    {
      console.log("delete row nr " + y);
      deleteRow(y);
    }
  }
}

function deleteRow(rowNr)
{
  var count = 0,
      data = collisionCtx.getImageData(0, 0, 10, 20);

  collisionCtx.putImageData(data, 0, 0, 0, 1, 20, rowNr);
  //collisionCtx.putImageData(data, 0, 1);
}

function drawCtx(ctx)
{
  ctx.save();
  ctx.fillStyle = colorBg;
  ctx.fillRect(0, 0, 10, 20);

  for(var i = 0; i < blocks.length; i++)
  {
    ctx.save();
    ctx.fillStyle = colorBlock;
    ctx.translate(blocks[i].x * 1 + 0.5, blocks[i].y + 0.5);
    ctx.rotate(blocks[i].angle * 0.01745329251); //Math.PI / 180
    ctx.translate(-0.5, -0.5);
    ctx.fill(blocks[i].mesh);
    ctx.restore();
  }
  ctx.restore();

  //console.log("collision:" + doesCollide());
  return doesCollide();
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

  var freezeCanvas = document.createElement('canvas');
  freezeCanvas.height = 20;
  freezeCanvas.width = 10;
  freezeContext = freezeCanvas.getContext('2d');

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

  blocks[0] = createTetrisBlock(0);
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

  if(this.counter >= 0.2)
  {
    activeBlock.y++;
    if(drawCtx(collisionCtx))
    {
      activeBlock.y--;
      drawCtx(collisionCtx);
      drawCtx(freezeContext);
      isARowFull();
      blocks[blocks.length] = createTetrisBlock(Math.floor(Math.random()*7));
      activeBlock = blocks[blocks.length - 1];
    }

    this.counter = 0;
  }

  //console.log(doesCollide());

  context.save();
  context.scale(blockSize, blockSize);
  context.drawImage(collisionCtx.canvas, 0, 0);
  //context.drawImage(freezeContext.canvas, 0, 0);
  context.restore();

  //$("#debug").html(deltaTime);
}
