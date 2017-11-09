//Fischer Thomas, Radlwimmer Stefan

//some init stuff
var gamePause = true,
    context,
    playWidth = 10,
    playHeight = 20,
    collisionContext,
    freezeContext,
    gameLoopInterval = undefined,
    startFrame = undefined,
    lastFrame = 0,
    stepTime = 0.5,
    counter = 0,
    debug = true,
    blockSize,
    blockStyles,
    blocks = [],
    activeBlock,
    blockCount = 0,
    colorBg = 'rgb(196, 207, 161)',
    colorBlock = 'rgb(107, 115, 83)',
    startBtn, leftBtn, rightBtn, downBtn, upBtn, escBtn,
    viewportOffset,//for mouse input on positioned viewport
    update = true;

function togglePause()
{
  if(debug)
    console.log("PAUSE!");
  gamePause = !gamePause;

  if(gamePause)
    showStartBtn();
}

function showStartBtn()
{
  context.save();
  context.fillStyle = colorBlock;
  context.fillRect(startBtn.x, startBtn.y, startBtn.width, startBtn.height);
  context.fillStyle = colorBg;
  context.textBaseline = "hanging";
  context.font = startBtn.height + 'px "VT323", monospace';
  var offset = {};
  offset.x = (startBtn.width - context.measureText("START").width) * 0.5;
  offset.y = 0;
  context.fillText("START", startBtn.x + offset.x, startBtn.y + offset.y);
  context.restore();
}

function drawControls()
{
  if(this.canvas == undefined)
  {
    if(debug)
      console.log("preparing UI canvas");

    this.canvas = document.createElement('canvas');
    this.canvas.height = context.canvas.height;
    this.canvas.width = context.canvas.height;
    this.ctx = this.canvas.getContext('2d');

    this.ctx.save();
    this.ctx.fillStyle = "rgba(0,0,0,0.3)";
    this.ctx.fillRect(leftBtn.x, leftBtn.y, leftBtn.width, leftBtn.height);
    this.ctx.fillRect(rightBtn.x, rightBtn.y, rightBtn.width, rightBtn.height);
    this.ctx.fillRect(downBtn.x, downBtn.y, downBtn.width, downBtn.height);
    this.ctx.fillRect(upBtn.x, upBtn.y, upBtn.width, upBtn.height);
    this.ctx.fillRect(escBtn.x, escBtn.y, escBtn.width, escBtn.height);

    var offset = {};
    this.ctx.fillStyle = "rgba(255,255,255,0.5)";
    this.ctx.textBaseline = "hanging";
    this.ctx.font = leftBtn.height + 'px "VT323", monospace';
    offset.x = (leftBtn.width - this.ctx.measureText("←").width) * 0.5;
    offset.y = 0;
    this.ctx.fillText("←", leftBtn.x + offset.x, leftBtn.y + offset.y);
    this.ctx.fillText("→", rightBtn.x + offset.x, rightBtn.y + offset.y);
    offset.x = (leftBtn.width - this.ctx.measureText("↓").width) * 0.5;
    offset.y = 0;
    this.ctx.fillText("↓", downBtn.x + offset.x, downBtn.y + offset.y);
    this.ctx.fillText("↑", upBtn.x + offset.x, upBtn.y + offset.y);
    offset.x = (leftBtn.width - this.ctx.measureText("X").width) * 0.5;
    offset.y = 0;
    this.ctx.fillText("X", escBtn.x + offset.x, escBtn.y + offset.y);
    this.ctx.restore();
  }

  context.save();
  context.drawImage(this.canvas, 0, 0);
  context.restore();
}

function contains(x, y, rect)
{
  return (x > rect.x
      && y > rect.y
      && x < rect.x + rect.width
      && y < rect.y + rect.height);
}

function createTetrisBlock(type)
{
  var tmp = {};
  tmp.x = 4;
  tmp.y = 1;
  tmp.angle = 0;
  tmp.style = type;
  tmp.mesh = blockStyles[type];
  blockCount += 4;

  return tmp;
}

function startGame()
{
  if(debug)
    $("#debug").show();
  else {
    $("#debug").hide();
  }

  viewport.off("click");
  viewport.click("click", click);

  counter = stepTime;
  update = true;
  togglePause();
}

function click(e)
{
  var mousePos = {};
  mousePos.x = e.clientX - viewportOffset.x;
  mousePos.y = e.clientY - viewportOffset.y;

  if(!gamePause)
  {
    if(contains(mousePos.x, mousePos.y, leftBtn))
    {
      moveActiveBlock("left");
    }
    else if(contains(mousePos.x, mousePos.y, rightBtn))
    {
      moveActiveBlock("right");
    }
    else if(contains(mousePos.x, mousePos.y, downBtn))
    {
      moveActiveBlock("down");
    }
    else if(contains(mousePos.x, mousePos.y, upBtn))
    {
      rotateActiveBlock();
    }
    else if(contains(mousePos.x, mousePos.y, escBtn))
    {
      context.save();
      context.scale(blockSize, blockSize);
      context.drawImage(collisionCtx.canvas, 0, 0);
      context.restore();

      togglePause();
    }

    if(debug)
      console.log("Jump!");
  }
  else {
    if(debug)
      console.log(mousePos.x + ":" + mousePos.y);
    if(contains(mousePos.x, mousePos.y, startBtn))
    {
      if(debug)
        console.log("START!");
      startGame();
    }
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
      if(activeBlock.x + 1 < playWidth)
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
      var moved = false;
      do{
        moved = true;
        activeBlock.y++;
      }while(!drawCtx(collisionCtx));

      if(moved)
      {
        activeBlock.y--;
        drawCtx(collisionCtx);
      }
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
      data = collisionCtx.getImageData(0, 0, playWidth, playHeight);

  for(var x = 0; x < playWidth; x++)
  {
    for(var y = 0; y < playHeight; y++)
    {
      var pixel = data.data[((y * (playWidth * 4)) + (x * 4))];
      if(pixel == colorBlock.substr(4,3))//compare red value
        count++;
    }
  }

  if(debug && count != blockCount)
    console.log("COLLISION!");

  return (count != blockCount);
}

function isARowFull()
{
  var count = 0,
      data = collisionCtx.getImageData(0, 0, playWidth, playHeight);

  for(var y = 0; y < playHeight; y++)
  {
    count = 0;
    for(var x = 0; x < playWidth; x++)
    {
      var pixel = data.data[((y * (playWidth * 4)) + (x * 4))];
      if(pixel == colorBlock.substr(4,3))//compare red value
        count++;
    }

    if(count == playWidth)
    {
      if(debug)
        console.log("delete row nr " + y);
      deleteRow(y);
    }
  }
}

function deleteRow(rowNr)
{
  freezeContext.save();
  freezeContext.drawImage(freezeContext.canvas, 0, 0, playWidth, rowNr, 0, 1, playWidth, rowNr);
  freezeContext.restore();

  blockCount -= playWidth;
}

function drawCtx(ctx)
{
  ctx.save();
  ctx.drawImage(freezeContext.canvas, 0, 0);
  ctx.restore();

  ctx.save();
  ctx.fillStyle = colorBlock;
  ctx.translate(activeBlock.x * 1 + 0.5, activeBlock.y + 0.5);
  ctx.rotate(activeBlock.angle * 0.01745329251); //Math.PI / 180
  ctx.translate(-0.5, -0.5);
  ctx.fill(activeBlock.mesh);
  ctx.restore();

  update = true;

  return doesCollide();
}

function clearCtx(ctx)
{
  ctx.fillStyle = colorBg;
  ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
}

//handler
$(document).ready(function() {
  init();
});

function init(){
  //Original Tetris: 10 blocks wide, 18 blocks high
  var canvas = document.getElementById('viewport');
  canvas.height = window.innerHeight;
  canvas.width = playWidth * canvas.height / playHeight;

  viewportOffset = {};
  viewportOffset.x = (window.innerWidth - canvas.width) * 0.5;
  viewportOffset.y = 0;
  $("#viewport").css("left", viewportOffset.x);

  context = canvas.getContext('2d');
  context.imageSmoothingEnabled = false;
  clearCtx(context);
  //https://stackoverflow.com/questions/43369748/how-to-render-offscreen-canvas-properly
  var collisionCanvas = document.createElement('canvas');
  collisionCanvas.height = playHeight;
  collisionCanvas.width = playWidth;
  collisionCtx = collisionCanvas.getContext('2d');

  var freezeCanvas = document.createElement('canvas');
  freezeCanvas.height = playHeight;
  freezeCanvas.width = playWidth;
  freezeContext = freezeCanvas.getContext('2d');

  blockSize = canvas.width / playWidth;

  blockStyles = [];
  //Line
  blockStyles[0] = new Path2D();
  for(var i = 0; i < 4; i++)
  {
    blockStyles[0].rect(0, -(i-1) * 1, 1, 1);
  }
  //L
  blockStyles[1] = new Path2D();
  blockStyles[1].rect(0, 1, 1, 1);
  blockStyles[1].rect(0, 0, 1, 1);
  blockStyles[1].rect(0, -1, 1, 1);
  blockStyles[1].rect(1, 1, 1, 1);
  //L2
  blockStyles[2] = new Path2D();
  blockStyles[2].rect(0, 1, 1, 1);
  blockStyles[2].rect(0, 0, 1, 1);
  blockStyles[2].rect(0, -1, 1, 1);
  blockStyles[2].rect(-1, 1, 1, 1);
  //var1
  blockStyles[3] = new Path2D();
  blockStyles[3].rect(0, 1,  1, 1);
  blockStyles[3].rect(0, 0, 1, 1);
  blockStyles[3].rect(-1, 0, 1, 1);
  blockStyles[3].rect(1, 1, 1, 1);
  //var2
  blockStyles[4] = new Path2D();
  blockStyles[4].rect(0, 1, 1, 1);
  blockStyles[4].rect(0, 0, 1, 1);
  blockStyles[4].rect(-1, 1, 1, 1);
  blockStyles[4].rect(1, 0, 1, 1);
  //cube
  blockStyles[5] = new Path2D();
  blockStyles[5].rect(0, 1, 1, 1);
  blockStyles[5].rect(0, 0, 1, 1);
  blockStyles[5].rect(1, 0, 1, 1);
  blockStyles[5].rect(1, 1, 1, 1);
  //var3
  blockStyles[6] = new Path2D();
  blockStyles[6].rect(0,-1,  1, 1);
  blockStyles[6].rect(0,0,1, 1);
  blockStyles[6].rect(-1,0,1, 1);
  blockStyles[6].rect(1, 0,  1, 1);

  startBtn = {};
  startBtn.width = 0.9 * context.canvas.width;
  startBtn.height = 0.3 * startBtn.width;
  startBtn.x = (context.canvas.width * 0.5) - (startBtn.width * 0.5);
  startBtn.y = (context.canvas.height * 0.5) - (startBtn.height * 0.5);

  var buttonSize = 0.25 * context.canvas.width;
  leftBtn = {};
  leftBtn.x = 0;
  leftBtn.y = context.canvas.height * 0.9;
  leftBtn.height = context.canvas.height * 0.1;
  leftBtn.width = buttonSize;

  rightBtn = {};
  rightBtn.x = buttonSize;
  rightBtn.y = context.canvas.height * 0.9;
  rightBtn.height = context.canvas.height * 0.1;
  rightBtn.width = buttonSize;

  upBtn = {};
  upBtn.x = buttonSize * 2;
  upBtn.y = context.canvas.height * 0.9;
  upBtn.height = context.canvas.height * 0.1;
  upBtn.width = buttonSize;

  downBtn = {};
  downBtn.x = buttonSize * 3;
  downBtn.y = context.canvas.height * 0.9;
  downBtn.height = context.canvas.height * 0.1;
  downBtn.width = buttonSize;

  escBtn = {};
  escBtn.x = 0;
  escBtn.y = 0;
  escBtn.height = context.canvas.height * 0.1;
  escBtn.width = buttonSize;

  $(document).click(click);

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
        {
          context.save();
          context.scale(blockSize, blockSize);
          context.drawImage(collisionCtx.canvas, 0, 0);
          context.restore();

          togglePause();
        }
      break;
    }
  });

  showStartBtn();

  //gameLoopInterval = setInterval(function(){gameLoop(500);}, 500);
  window.requestAnimationFrame(requestFrameHandler);
}

function startGame()
{
  if(debug)
    console.log("starting game...");
  clearCtx(context);
  clearCtx(collisionCtx);
  clearCtx(freezeContext);
  blocks = [];
  blocks[0] = createTetrisBlock(0);
  activeBlock = blocks[0];
  blockCount = 4;
  togglePause();
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

  window.requestAnimationFrame(requestFrameHandler);
}

function gameLoop(deltaTime)
{
  if(gamePause)
    return;

  counter += deltaTime;

  if(counter >= stepTime)
  {
    activeBlock.y++;
    if(drawCtx(collisionCtx))
    {
      activeBlock.y--;
      if(drawCtx(collisionCtx))
      {
        context.save();
        context.scale(blockSize, blockSize);
        context.drawImage(collisionCtx.canvas, 0, 0);
        context.restore();

        togglePause();
        return;
      }

      drawCtx(freezeContext);
      isARowFull();
      blocks[blocks.length] = createTetrisBlock(Math.floor(Math.random()*7));
      activeBlock = blocks[blocks.length - 1];
    }

    counter = 0;
  }

  if(update)
  {
    context.save();
    context.scale(blockSize, blockSize);
    context.drawImage(collisionCtx.canvas, 0, 0);
    context.restore();

    drawControls();
    update = false;
  }
}
