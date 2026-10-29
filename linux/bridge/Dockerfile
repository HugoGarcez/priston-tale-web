FROM node:20-bookworm-slim
WORKDIR /app
COPY package.json package-lock.json ./
RUN npm ci --omit=dev
COPY bridge.js ./
USER node
CMD ["node", "bridge.js"]
