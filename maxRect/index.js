let MaxRectsPacker = require("maxrects-packer").MaxRectsPacker;
let Rectangle = require("maxrects-packer").Rectangle;
let generateTasks = require("./tasks").generateTasks;
const fs = require("fs");
const tasks = require("./data/items.json");
const paletteSizes = require("./data/paletteSizes.json");

const options = {
  smart: true,
  pot: false,
  square: true,
  allowRotation: false,
  tag: false,
}; // Set packing options

const tasksCount = tasks.length;
// const tasksCount = 1;

for (let taskIndex = 0; taskIndex < tasksCount; taskIndex++) {
  const input = [];

  const task = tasks[taskIndex];
  const paletteSize = paletteSizes[taskIndex];

  const itemsCount = task.length;

  for (let itemIndex = 0; itemIndex < itemsCount; itemIndex++) {
    const item = task[itemIndex];

    const width = item[0];
    const height = item[1];
    const count = item[2];

    for (let i = 0; i < count; i++) {
      input.push(new Rectangle(width, height));
    }
  }

  const packer = new MaxRectsPacker(paletteSize[0], paletteSize[1], 0, options); // width, height, padding, options
  packer.addArray(input); // Start packing with input array

  const firstBinRectangles = packer.bins[0].rects;
  const packedSize = firstBinRectangles.reduce(
    (prev, curr) => curr.width * curr.height + prev,
    0
  );
  const allItemsSize = input.reduce(
    (prev, curr) => curr.width * curr.height + prev,
    0
  );

  console.log("task", taskIndex);
  console.log("Result", packedSize / allItemsSize, "\n");
}
