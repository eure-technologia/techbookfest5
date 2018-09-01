FROM vvakame/review:2.4
RUN useradd boxp \
  && mkdir -p /home/boxp \
  && chown -R boxp:boxp /home/boxp \
  && gem install bundler rake --no-rdoc --no-ri \
  && gem install review -v "$REVIEW_VERSION" --no-rdoc --no-ri \
  && gem install review-peg -v "$REVIEW_PEG_VERSION" --no-rdoc --no-ri \
  && chmod -R a+rw /var/lib/gems /usr/local/bin
USER boxp
WORKDIR /book
