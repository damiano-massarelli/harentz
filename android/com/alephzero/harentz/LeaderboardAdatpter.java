package com.alephzero.harentz;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class LeaderboardAdatpter extends RecyclerView.Adapter {

    private JSONArray data;

    public LeaderboardAdatpter(JSONArray data) {
        super();
        this.data = data;
        notifyDataSetChanged();
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return Holder.inflate(parent);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        JSONObject entry = null;
        try {
            entry = data.getJSONObject(position);
            if (holder instanceof Holder) {
                ((Holder) holder).bind(entry.getString("username"), entry.getString("bestScore"), entry.getString("rank"));
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int getItemCount() {
        return data.length();
    }

    static class Holder extends RecyclerView.ViewHolder {

        private TextView username, score, rank;

        private static Holder inflate(ViewGroup parent) {
            View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.score_view_layout, parent, false);
            return new Holder(view);
        }

        public Holder(View itemView) {
            super(itemView);
            username = itemView.findViewById(R.id.username);
            score = itemView.findViewById(R.id.score);
            rank = itemView.findViewById(R.id.rank);
        }

        private void bind(String username, String score, String rank) {
            this.username.setText(username);
            this.score.setText(score);
            this.rank.setText(rank);
        }
    }
}
